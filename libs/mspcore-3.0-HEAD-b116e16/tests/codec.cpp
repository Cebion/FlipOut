#include <msp/stringcodec/ascii.h>
#include <msp/stringcodec/utf8.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class CodecTests: public Test::RegisteredTest<CodecTests>
{
public:
	CodecTests();

	static const char *get_name() { return "Codec"; }

private:
	void invalid_character();
	void invalid_sequence();
};


CodecTests::CodecTests()
{
	add(&CodecTests::invalid_character, "invalid_character").expect_throw<StringCodec::invalid_character>();
	add(&CodecTests::invalid_sequence, "invalid_sequence").expect_throw<StringCodec::invalid_sequence>();
}

void CodecTests::invalid_character()
{
	StringCodec::unichar ch = 0xE4;
	StringCodec::Ascii::Encoder enc;
	string buf;
	enc.encode_char(ch, buf);
}

void CodecTests::invalid_sequence()
{
	string str = "\343\201";
	string::const_iterator iter = str.begin();
	StringCodec::Utf8::Decoder dec;
	dec.decode_char(str, iter);
}
