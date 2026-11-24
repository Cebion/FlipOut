#ifndef MSP_STRINGCODEC_ISO885915_H_
#define MSP_STRINGCODEC_ISO885915_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class Iso885915: public StandardCodec<Iso885915>
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

	Iso885915(ErrorMode em = DEFAULT): StandardCodec<Iso885915>(em) { }

	virtual const char *get_name() const { return "ISO-8859-15"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
