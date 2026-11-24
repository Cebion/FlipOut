#ifndef MSP_STRINGCODEC_ASCII_H_
#define MSP_STRINGCODEC_ASCII_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class Ascii: public StandardCodec<Ascii>
{
public:
	class Encoder: public Codec::Encoder
	{
	public:
		Encoder(ErrorMode em = DEFAULT): Codec::Encoder(em) { }

		virtual void encode_char(unichar, std::string &);
	private:
		virtual void transliterate(unichar, std::string &);
	};

	class Decoder: public Codec::Decoder
	{
	public:
		Decoder(ErrorMode em = DEFAULT): Codec::Decoder(em) { }

		virtual unichar decode_char(const std::string &, std::string::const_iterator &);
	};

	Ascii(ErrorMode em = DEFAULT): StandardCodec<Ascii>(em) { }

	virtual const char *get_name() const { return "ASCII"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
