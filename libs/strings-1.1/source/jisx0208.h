/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_JISX0208_H_
#define MSP_STRINGS_JISX0208_H_

#include "codec.h"

namespace Msp {
namespace Codecs {

/**
Codec for the JIS X 0208 encoding.  This is not particularly useful as a
stand-alone codec, due to lack of a linefeed character among other things,
but is included as part of some other encodings.
*/
class JisX0208: public Codec
{
public:
	class Encoder: public Codec::Encoder
	{
	public:
		Encoder(ErrorMode em=THROW_ON_ERROR): Codec::Encoder(em) { }
		virtual void encode_char(UnicodeChar, std::string &);
	private:
		virtual void transliterate(UnicodeChar, std::string &);
	};

	class Decoder: public Codec::Decoder
	{
	public:
		Decoder(ErrorMode em=THROW_ON_ERROR): Codec::Decoder(em) { }
		virtual UnicodeChar decode_char(const std::string &, std::string::const_iterator &);
	};

	virtual const char *get_name() const { return "JIS X 0208"; }

	virtual Encoder *create_encoder(ErrorMode em=THROW_ON_ERROR) const { return new Encoder(em); }
	virtual Decoder *create_decoder(ErrorMode em=THROW_ON_ERROR) const { return new Decoder(em); }
};

struct Kuten
{
	unsigned short ku;
	unsigned short ten;

	Kuten(): ku(0), ten(0) { }
	operator bool() { return ku!=0 && ten!=0; }
};

extern UnicodeChar jisx0208_to_ucs(Kuten);
extern Kuten ucs_to_jisx0208(UnicodeChar);

} // namespace Codecs
} // namespace Msp

#endif
