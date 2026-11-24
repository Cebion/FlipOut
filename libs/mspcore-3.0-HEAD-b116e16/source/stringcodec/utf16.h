#ifndef MSP_STRINGCODEC_UTF16_H_
#define MSP_STRINGCODEC_UTF16_H_

#include "codec.h"

namespace Msp {
namespace StringCodec {

/**
The UTF-16 codec, as specified in the Unicode standard.  Both little and big
endian are supported, as well as autodetection with the BOM.  In the absence
of a BOM, big endian is assumed.
*/
class Utf16: public StandardCodec<Utf16>
{
public:
	enum Endian
	{
		AUTO,
		BIG,
		LITTLE
	};

	class Encoder: public Codec::Encoder
	{
	private:
		Endian endian;
		bool emit_bom;

	public:
		Encoder(ErrorMode em = DEFAULT, Endian en = BIG);

		virtual void encode_char(unichar, std::string &);
	private:
		virtual void transliterate(unichar, std::string &);
	};

	class Decoder: public Codec::Decoder
	{
	private:
		Endian endian;

	public:
		Decoder(ErrorMode em = DEFAULT, Endian en = AUTO);

		virtual unichar decode_char(const std::string &, std::string::const_iterator &);
	private:
		unichar decode_unit(const std::string &, const std::string::const_iterator &, std::string::const_iterator &);
	};

private:
	Endian endian;

public:
	Utf16(ErrorMode em = DEFAULT, Endian en = AUTO):
		StandardCodec<Utf16>(em),
		endian(en)
	{ }

	virtual const char *get_name() const
	{ return endian==BIG ? "UTF-16-BE" : endian==LITTLE ? "UTF-16-LE" : "UTF-16"; }

	virtual Encoder *create_encoder(ErrorMode em = DEFAULT) const
	{ return new Encoder(get_error_mode(em), endian); }

	virtual Decoder *create_decoder(ErrorMode em = DEFAULT) const
	{ return new Decoder(get_error_mode(em), endian); }
};


/**
A helper template to define the Utf16Be and Utf16Le types.
*/
template<Utf16::Endian en>
class Utf16E: public Utf16
{
public:
	class Encoder: public Utf16::Encoder
	{
	public:
		Encoder(ErrorMode em = DEFAULT): Utf16::Encoder(em, en) { }
	};

	class Decoder: public Utf16::Decoder
	{
	public:
		Decoder(ErrorMode em = DEFAULT): Utf16::Decoder(em, en) { }
	};

	Utf16E(ErrorMode em = DEFAULT): Utf16(em, en) { }
};

typedef Utf16E<Utf16::BIG> Utf16Be;
typedef Utf16E<Utf16::LITTLE> Utf16Le;

} // namespace StringCodec
} // namespace Msp

#endif
