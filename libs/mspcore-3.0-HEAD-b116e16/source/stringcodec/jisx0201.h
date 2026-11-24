#ifndef MSP_STRINGCODEC_JISX201_H_
#define MSP_STRINGCODEC_JISX201_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class JisX0201: public StandardCodec<JisX0201>
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

	JisX0201(ErrorMode em = DEFAULT): StandardCodec<JisX0201>(em) { }

	virtual const char *get_name() const { return "JIS X 0201"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
