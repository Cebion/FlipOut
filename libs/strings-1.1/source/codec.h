/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_CODEC_H_
#define MSP_STRINGS_CODEC_H_

#include <string>
#include <msp/core/except.h>

namespace Msp {
namespace Codecs {

typedef int UnicodeChar;

typedef std::basic_string<UnicodeChar> ustring;

enum ErrorMode
{
	THROW_ON_ERROR,
	IGNORE_ERRORS,
	TRANSLITERATE
};

/**
An exception thrown for all kinds of problems encountered while encoding or
decoding strings.
*/
class CodecError: public Exception
{
public:
	CodecError(const std::string &w_): Exception(w_) { }
};

/**
Base class for string codecs.  Use one of the derived classes or the function
create_codec to create a specific codec.

Unicode strings are represented as ustrings.  An std::string is considered to
be an encoded sequence of bytes.  A codec is able to determine if an encoded
string could be decoded with it.
*/
class Codec
{
public:
	/**
	Base class for string encoder.

	Each codec class should contain an Encoder class derived from this.  The
	encode_char and transliterate functions must be overloaded.  Some encoders
	may find it useful or necessary to implement some other functions too
	(particularly sync and reset for stateful codecs).
	*/
	class Encoder
	{
	public:
		virtual ~Encoder() { }

		/**
		Encodes a single unicode character.  If the character can't be
		represented in this encoding, behavior depends on the error mode
		specified for the encoder:

		For THROW_ON_ERROR, an exception is thrown.

		For IGNORE_ERRORS, nothing is done.

		For TRANSLITERATE, the encoder attempts to select a character or a string
		or characters that closely approximates the non-representable character.
		*/
		virtual void encode_char(UnicodeChar ch, std::string &buf) =0;

		/**
		Encodes a unicode string.  This is equivalent to callind encode_char for
		each character in the string with the same buffer.
		*/
		virtual void encode(const ustring &str, std::string &buf);

		std::string encode(const ustring &);

		/**
		Procuces a sequence of bytes that will bring the encoder back to the
		initial state.
		*/
		virtual void sync(std::string &buf) { (void)buf; }

		/**
		Resets the encoder to the initial state without producing output.
		*/
		virtual void reset() { }
	protected:
		ErrorMode err_mode;

		Encoder(ErrorMode em): err_mode(em) { }

		/**
		Handles an error depending on the error mode.

		For THROW_ON_ERROR, throws CodecError(msg).

		For IGNORE_ERROR, does nothing.

		For TRANSLITERATE, calls transliterate(ch, buf).
		*/
		void error(UnicodeChar ch, std::string &buf, const std::string &msg);

		/**
		Attempts to produce an alternative encoding for a unicode character.
		Typically this includes dropping accent marks or romanizing letters.
		*/
		virtual void transliterate(UnicodeChar ch, std::string &buf) =0;
	};

	/**
	Base class for string decoder.

	Each codec class should contain an Decoder class derived from this.
	*/
	class Decoder
	{
	public:
		virtual ~Decoder() { }

		/**
		Decodes a single character from a string.  The iterator is advanced to
		the next character.  For stateful codecs, -1 may be returned if a state
		change sequence was decoded but no character followed it.  In case a
		decoding error occurs, behavior depends on the error mode specified for
		the decoder:

		For THROW_ON_ERROR, an exception is thrown and the iterator is left at
		the erroneous character.

		For IGNORE_ERRORS, -1 is returned and the iterator is advanced.

		For TRANSLITERATE, 0xFFFE is returned and the iterator is advanced.
		*/
		virtual UnicodeChar decode_char(const std::string &str, std::string::const_iterator &i) =0;

		/**
		Decodes a string.
		*/
		virtual void decode(const std::string &str, ustring &buf);

		ustring decode(const std::string &);

		/**
		Resets the decoder to the initial state.
		*/
		virtual void reset() { }
	protected:
		ErrorMode err_mode;

		Decoder(ErrorMode em): err_mode(em) { }

		/**
		Handles an error depending on the error mode.
		*/
		UnicodeChar error(const std::string &);
	};

	virtual ~Codec() { }

	/**
	Returns the name of the encoding handled by this codec.
	*/
	virtual const char *get_name() const =0;

	/**
	Creates an encoder for this codec.
	*/
	virtual Encoder *create_encoder(ErrorMode err_mode=THROW_ON_ERROR) const =0;

	/**
	Creates a decoder for this codec.
	*/
	virtual Decoder *create_decoder(ErrorMode err_mode=THROW_ON_ERROR) const =0;

	/**
	Determines whether the given string can be successfully decoded with this
	codec.  Note that this function returning true does not guarantee that the
	string was actually encoded with this codec.  In particular, many 8-bit
	encodings are indistinguishable.
	*/
	virtual bool detect(const std::string &) const;
protected:
	Codec() { }
};

typedef Codec::Encoder Encoder;
typedef Codec::Decoder Decoder;

/**
Convenience function that decodes a string.
*/
template<class C>
ustring decode(const std::string &s)
{
	typename C::Decoder dec;
	ustring result;
	dec.decode(s, result);
	return result;
}

/**
Convenience function that encodes a string.
*/
template<class C>
std::string encode(const ustring &s)
{
	typename C::Encoder enc;
	std::string result;
	enc.encode(s, result);
	enc.sync(result);
	return result;
}

/**
Convenience function that transcodes a string from one codec to another.
*/
template<class F, class T>
std::string transcode(const std::string &s)
{
	typename F::Decoder from;
	typename T::Encoder to;
	ustring temp;
	from.decode(s, temp);
	std::string result;
	to.encode(temp, result);
	to.sync(result);
	return result;
}

/**
Creates a codec for an encoding by name.  The caller is responsible for
deleting the codec when it's no longer needed.
*/
Codec *create_codec(const std::string &);

/**
Automatically detects the encoding of a string and creates a codec for it.
The codec must be deleted when it's no longer needed.
*/
Codec *detect_codec(const std::string &);

} // namespace Codecs
} // namespace Msp

#endif
