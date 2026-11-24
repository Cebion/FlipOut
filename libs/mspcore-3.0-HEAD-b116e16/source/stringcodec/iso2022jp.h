#ifndef MSP_STRINGCODEC_ISO2022JP_H_
#define MSP_STRINGCODEC_ISO2022JP_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

class Iso2022Jp: public StandardCodec<Iso2022Jp>
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
	private:
		Mode mode;

	public:
		Encoder(ErrorMode em = DEFAULT): Codec::Encoder(em), mode(ASCII) { }

		virtual void encode_char(unichar, std::string &);
		virtual void sync(std::string &);
		virtual void reset();
	private:
		void switch_mode(Mode, std::string &);
		virtual void transliterate(unichar, std::string &);
	};

	class Decoder: public Codec::Decoder
	{
	private:
		Mode mode;
		Codec::Decoder *dec;

	public:
		Decoder(ErrorMode = DEFAULT);

		virtual unichar decode_char(const std::string &, std::string::const_iterator &);
		virtual void reset();
	private:
		void switch_mode(Mode);
	};

	Iso2022Jp(ErrorMode em = DEFAULT): StandardCodec<Iso2022Jp>(em) { }

	virtual const char *get_name() const { return "ISO-2022-JP"; }
};

} // namespace StringCodec
} // namespace Msp

#endif
