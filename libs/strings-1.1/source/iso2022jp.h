/* $Id$

This file is part of libmspstrings
Copyright © 2006-2007 Mikko Rasa
Distributed under the LGPL
*/

#ifndef MSP_STRINGS_ISO2022JP_H_
#define MSP_STRINGS_ISO2022JP_H_

#include "codec.h"

namespace Msp {
namespace Codecs {

class Iso2022Jp: public Codec
{
public:
	enum Mode
	{
		ASCII,
		JISX0201,
		JISX0208
	};

	class Encoder: public Codec::Encoder
	{
	public:
		Encoder(ErrorMode em=THROW_ON_ERROR): Codec::Encoder(em), mode(ASCII) { }
		virtual void encode_char(UnicodeChar, std::string &);
		virtual void sync(std::string &);
		virtual void reset();
	private:
		Mode mode;

		void switch_mode(Mode, std::string &);
		virtual void transliterate(UnicodeChar, std::string &);
	};

	class Decoder: public Codec::Decoder
	{
	public:
		Decoder(ErrorMode =THROW_ON_ERROR);
		virtual UnicodeChar decode_char(const std::string &, std::string::const_iterator &);
		virtual void reset();
	private:
		Mode mode;
		Codec::Decoder *dec;

		virtual void switch_mode(Mode);
	};

	virtual const char *get_name() const { return "ISO-2022-JP"; }

	virtual Encoder *create_encoder(ErrorMode em=THROW_ON_ERROR) const { return new Encoder(em); }
	virtual Decoder *create_decoder(ErrorMode em=THROW_ON_ERROR) const { return new Decoder(em); }
};

} // namespace Codecs
} // namespace Msp

#endif
