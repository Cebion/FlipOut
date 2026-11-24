#ifndef MSP_STRINGCODEC_ISO646FI_H_
#define MSP_STRINGCODEC_ISO646FI_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class Iso646Fi: public StandardCodec<Iso646Fi>
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

	Iso646Fi(ErrorMode em = DEFAULT): StandardCodec<Iso646Fi>(em) { }

	virtual const char *get_name() const { return "ISO-646-FI"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
