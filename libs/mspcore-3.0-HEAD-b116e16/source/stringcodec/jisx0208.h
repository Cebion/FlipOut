#ifndef MSP_STRINGCODEC_JISX0208_H_
#define MSP_STRINGCODEC_JISX0208_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

/**
Codec for the JIS X 0208 encoding.  This is not particularly useful as a
stand-alone codec, due to lack of a linefeed character among other things,
but is included as part of some other encodings.
*/
class JisX0208: public StandardCodec<JisX0208>
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

	JisX0208(ErrorMode em = DEFAULT): StandardCodec<JisX0208>(em) { }

	virtual const char *get_name() const { return "JIS X 0208"; }
};


struct Kuten
{
	unsigned short ku;
	unsigned short ten;

	Kuten(): ku(0), ten(0) { }

	operator bool() { return ku!=0 && ten!=0; }
};

unichar jisx0208_to_ucs(Kuten);
Kuten ucs_to_jisx0208(unichar);

} // namespace StringCodec
} // namespace Msp

#endif
