#ifndef MSP_STRINGCODEC_UTF8_H_
#define MSP_STRINGCODEC_UTF8_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class Utf8: public StandardCodec<Utf8>
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

	Utf8(ErrorMode em = DEFAULT): StandardCodec<Utf8>(em) { }

	virtual const char *get_name() const { return "UTF-8"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
