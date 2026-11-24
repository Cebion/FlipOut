#include <msp/strings/utils.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class StringUtilsTests: public Test::RegisteredTest<StringUtilsTests>
{
private:
	static const char *strings[];

public:
	StringUtilsTests();

	static const char *get_name() { return "stringutils"; }

private:
	void escape();
	void unescape();
	void unescape_invalid();
	void split();
};

const char *StringUtilsTests::strings[] =
{
	"new\nline", "new\\nline",
	"\ttab", "\\ttab",
	"\"quotes'", "\\\"quotes\\'",
	"back\\slash", "back\\\\slash",
	"\001ACTION ducks\001", "\\001ACTION ducks\\001",
	"g\303\266tterd\303\244mmerung", "g\\303\\266tterd\\303\\244mmerung",
	0
};

StringUtilsTests::StringUtilsTests()
{
	add(&StringUtilsTests::escape, "c_escape");
	add(&StringUtilsTests::unescape, "c_unescape");
	add(&StringUtilsTests::unescape_invalid, "c_unescape (invalid)").expect_throw<invalid_argument>();
	add(&StringUtilsTests::split, "split");
}

void StringUtilsTests::escape()
{
	for(const char **i=strings; *i; i+=2)
	{
		string str = c_escape(*i);
		expect_equal(str, str==*(i+1), format("str == '%s'", *(i+1)));
	}
}

void StringUtilsTests::unescape()
{
	for(const char **i=strings; *i; i+=2)
	{
		string str = c_unescape(*(i+1));
		expect_equal(str, str==*i, format("str == '%s'", *i));
	}
}

void StringUtilsTests::unescape_invalid()
{
	c_unescape("\\q");
}

void StringUtilsTests::split()
{
	vector<string> parts = ::split("foo bar  baz");
	EXPECT_EQUAL(parts.size(), 3);
	EXPECT_EQUAL(parts[0], "foo");
	EXPECT_EQUAL(parts[1], "bar");
	EXPECT_EQUAL(parts[2], "baz");

	parts = split_fields("one::three", ':');
	EXPECT_EQUAL(parts.size(), 3);
	EXPECT_EQUAL(parts[0], "one");
	EXPECT(parts[1].empty());
	EXPECT_EQUAL(parts[2], "three");
}
