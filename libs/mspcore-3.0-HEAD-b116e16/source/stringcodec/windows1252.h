#ifndef MSP_STRINGCODEC_WINDOWS1252_H_
#define MSP_STRINGCODEC_WINDOWS1252_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class Windows1252: public StandardCodec<Windows1252>
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

	Windows1252(ErrorMode em = DEFAULT): StandardCodec<Windows1252>(em) { }

	virtual const char *get_name() const { return "Windows-1252"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
