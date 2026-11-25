/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_ASCII_H_
#define MSP_STRINGS_ASCII_H_

#include "codec.h"

namespace Msp {
namespace Codecs {

class Ascii: public Codec
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

	virtual const char *get_name() const { return "ASCII"; }

	virtual Encoder *create_encoder(ErrorMode em=THROW_ON_ERROR) const { return new Encoder(em); }
	virtual Decoder *create_decoder(ErrorMode em=THROW_ON_ERROR) const { return new Decoder(em); }
};

} // namespace Codecs
} // namespace Msp

#endif
