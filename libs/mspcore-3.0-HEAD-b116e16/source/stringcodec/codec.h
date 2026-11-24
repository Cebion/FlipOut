#ifndef MSP_STRINGCODEC_CODEC_H_
#define MSP_STRINGCODEC_CODEC_H_

#include <string>
#include "except.h"
#include "ustring.h"

namespace Msp {
namespace StringCodec {

enum ErrorMode
{
	DEFAULT,
	THROW_ON_ERROR,
	IGNORE_ERRORS,
	TRANSLITERATE
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
	protected:
		ErrorMode err_mode;

		Encoder(ErrorMode em): err_mode(em==DEFAULT ? THROW_ON_ERROR : em) { }
	public:
		virtual ~Encoder() { }

		/** Encodes a single unicode character.  If the character can't be
		represented in this encoding, error() should be called. */
		virtual void encode_char(unichar ch, std::string &buf) = 0;

		/** Encodes a unicode string.  This is equivalent to calling encode_char
		for each character in the string with the same buffer. */
		virtual void encode(const ustring &str, std::string &buf);

		std::string encode(const ustring &);

		/** Procuces a sequence of bytes that will bring the encoder back to the
		initial state. */
		virtual void sync(std::string &buf) { (void)buf; }

		/** Resets the encoder to the initial state without producing output. */
		virtual void reset() { }

	protected:
		/** Handles an error depending on the error mode.

		THROW_ON_ERROR: throws err
		IGNORE_ERRORS: does nothing
		TRANSLITERATE: calls transliterate(ch, buf) */
		template<typename E>
		void error(unichar ch, std::string &buf, const E &err)
		{
			if(err_mode==TRANSLITERATE)
				transliterate(ch, buf);
			else if(err_mode!=IGNORE_ERRORS)
				throw err;
		}

		/** Attempts to produce an alternative encoding for a unicode character.
		Typically this includes dropping accent marks or romanizing letters. */
		virtual void transliterate(unichar ch, std::string &buf) = 0;
	};

	/**
	Base class for string decoder.

	Each codec class should contain an Decoder class derived from this.
	*/
	class Decoder
	{
	protected:
		ErrorMode err_mode;

		Decoder(ErrorMode em): err_mode(em==DEFAULT ? THROW_ON_ERROR : em) { }
	public:
		virtual ~Decoder() { }

		/** Decodes a single character from a string.  The iterator is advanced
		to the next character.  For stateful codecs, -1 may be returned if a
		state change sequence was decoded but no character followed it.  If
		invalid input is encountered, the error() function should be called and
		the iterator advanced only if it doesn't throw. */
		virtual unichar decode_char(const std::string &str, std::string::const_iterator &i) = 0;

		/** Decodes a string. */
		virtual void decode(const std::string &str, ustring &buf);

		ustring decode(const std::string &);

		/** Resets the decoder to the initial state. */
		virtual void reset() { }

	protected:
		/** Handles an error depending on the error mode.  The return value is
		suitable for returning from decode_char.
		
		THROW_ON_ERROR: throws err
		IGNORE_ERRORS: returns -1
		TRANSLITERATE: return 0xFFFD */
		template<typename E>
		unichar error(const E &err)
		{
			if(err_mode==TRANSLITERATE)
				return 0xFFFD;
			else if(err_mode==IGNORE_ERRORS)
				return -1;
			else
				throw err;
		}
	};

protected:
	Codec() { }
public:
	virtual ~Codec() { }

	/** Returns the name of the encoding handled by this codec. */
	virtual const char *get_name() const = 0;

	/** Creates an encoder for this codec. */
	virtual Encoder *create_encoder(ErrorMode err_mode = DEFAULT) const = 0;

	/** Creates a decoder for this codec. */
	virtual Decoder *create_decoder(ErrorMode err_mode = DEFAULT) const = 0;

	/** Determines whether the given string can be successfully decoded with
	this codec.  Note that this function returning true does not guarantee that
	the string was actually encoded with this codec.  In particular, many 8-bit
	encodings are indistinguishable. */
	virtual bool detect(const std::string &) const;
};

typedef Codec::Encoder Encoder;
typedef Codec::Decoder Decoder;


/**
A helper class to provide some common functionality.
*/
template<typename C>
class StandardCodec: public Codec
{
private:
	ErrorMode err_mode;

protected:
	StandardCodec(ErrorMode em): err_mode(em==DEFAULT ? THROW_ON_ERROR : em) { }

	ErrorMode get_error_mode(ErrorMode em = DEFAULT) const
	{ return (em==DEFAULT ? err_mode : em); }

public:
	virtual Encoder *create_encoder(ErrorMode em = DEFAULT) const
	{ return new typename C::Encoder(get_error_mode(em)); }

	virtual Decoder *create_decoder(ErrorMode em = DEFAULT) const
	{ return new typename C::Decoder(get_error_mode(em)); }
};


/** Convenience function that decodes a string. */
template<typename C>
ustring decode(const std::string &s)
{
	typename C::Decoder dec;
	return dec.decode(s);
}

/** Convenience function that encodes a string. */
template<typename C>
std::string encode(const ustring &s)
{
	typename C::Encoder enc;
	return enc.encode(s);
}

/** Convenience function that transcodes a string from one codec to another. */
template<typename F, typename T>
std::string transcode(const std::string &s)
{
	return encode<T>(decode<F>(s));
}

/** Creates a codec for an encoding by name.  The caller is responsible for
deleting the codec when it's no longer needed. */
Codec *create_codec(const std::string &);

/** Automatically detects the encoding of a string and creates a codec for it.
The codec must be deleted when it's no longer needed. */
Codec *detect_codec(const std::string &);

} // namespace StringCodec
} // namespace Msp

#endif
