#include <limits>
#include <msp/core/inttypes.h>
#include <msp/strings/utils.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class LexicalCastTests: public Test::RegisteredTest<LexicalCastTests>
{
private:
	template<typename T>
	struct Value
	{
		T value;
		const char *format;
		const char *result;
	};

public:
	LexicalCastTests();

	static const char *get_name() { return "lexical_cast"; }

private:
	void basic_integers();
	void hex_integers();
	void oct_integers();
	void bin_integers();
	void integer_limits();
	void float_fixed();
	void float_auto();
	void float_sci();
	void float_corner_cases();

	template<typename T>
	void test_values(const Value<T> *);
};

LexicalCastTests::LexicalCastTests()
{
	add(&LexicalCastTests::basic_integers, "Basic integers");
	add(&LexicalCastTests::hex_integers, "Hexadecimal integers");
	add(&LexicalCastTests::oct_integers, "Octal integers");
	add(&LexicalCastTests::bin_integers, "Binary integers");
	add(&LexicalCastTests::integer_limits, "Integer limits");
	add(&LexicalCastTests::float_fixed, "Fixed-precision floats");
	add(&LexicalCastTests::float_auto, "Automatic floats");
	add(&LexicalCastTests::float_sci, "Scientific floats");
	add(&LexicalCastTests::float_corner_cases, "Float corner cases");
}

void LexicalCastTests::basic_integers()
{
	Value<int> values[] =
	{
		{ 0, "%d", "0" },
		{ 1, "%4d", "   1" },
		{ 1, "%04d", "0001" },
		{ 1, "%-04d", "1   " },
		{ 1, "%+4d", "  +1" },
		{ 1, "%+04d", "+001" },
		{ 1, "%+-4d", "+1  " },
		{ -1, "%d", "-1" },
		{ -1, "%04d", "-001" },
		{ 999999999, "%d", "999999999" },
		{ 1000000000, "%d", "1000000000" },
		{ 1234567890, "%d", "1234567890" },
		{ 0, 0, 0 }
	};
	test_values<int>(values);
}

void LexicalCastTests::hex_integers()
{
	Value<int> values[] =
	{
		{ 0xabcd, "%x", "abcd" },
		{ 0xabcd, "%#x", "0xabcd" },
		{ 0xabcd, "%#8x", "  0xabcd" },
		{ 0xabcd, "%-#8x", "0xabcd  " },
		{ 0xabcd, "%#08x", "0x00abcd" },
		{ 0xabcd, "%#X", "0XABCD" },
		{ 0xabcd, "%+#x", "+0xabcd" },
		{ -0xabcd, "%#x", "-0xabcd" },
		{ 0, 0, 0 }
	};
	test_values<int>(values);
}

void LexicalCastTests::oct_integers()
{
	Value<int> values[] =
	{
		{ 012345, "%o", "12345" },
		{ 012345, "%#o", "012345" },
		{ 012345, "%#8o", "  012345" },
		{ 012345, "%-#8o", "012345  " },
		{ 012345, "%#08o", "00012345" },
		{ 012345, "%+#o", "+012345" },
		{ -012345, "%#o", "-012345" },
		{ 0, 0, 0 }
	};
	test_values<int>(values);
}

void LexicalCastTests::bin_integers()
{
	Value<int> values[] =
	{
		{ 0xcf3, "%b", "110011110011" },
		{ 0xcf3, "%#b", "0b110011110011" },
		{ 0xcf3, "%#16b", "  0b110011110011" },
		{ 0xcf3, "%-#16b", "0b110011110011  " },
		{ 0xcf3, "%#016b", "0b00110011110011" },
		{ 0xcf3, "%+#b", "+0b110011110011" },
		{ -0xcf3, "%#b", "-0b110011110011" },
		{ 0, 0, 0 }
	};
	test_values<int>(values);
}

void LexicalCastTests::integer_limits()
{
	Value<Int32> int32_values[] =
	{
		{ numeric_limits<Int32>::max(), "%d", "2147483647" },
		{ numeric_limits<Int32>::max(), "%x", "7fffffff" },
		{ numeric_limits<Int32>::min(), "%d", "-2147483648" },
		{ numeric_limits<Int32>::min(), "%x", "-80000000" },
		{ 0, 0, 0 }
	};
	test_values<Int32>(int32_values);

	Value<UInt32> uint32_values[] =
	{
		{ numeric_limits<UInt32>::max(), "%d", "4294967295" },
		{ numeric_limits<UInt32>::max(), "%x", "ffffffff" },
		{ 0, 0, 0 }
	};
	test_values<UInt32>(uint32_values);
}

void LexicalCastTests::float_fixed()
{
	Value<float> float_values[] =
	{
		{ 0, "%f", "0.000000" },
		{ 1, "%.0f", "1" },
		{ 1, "%#.0f", "1." },
		{ 1, "%06.2f", "001.00" },
		{ 1, "%.1f", "1.0" },
		{ 0.0987, "%f", "0.098700" },
		{ 0.0987, "%.3f", "0.099" },
		{ 0.0987, "%.2f", "0.10" },
		{ 0, 0, 0 }
	};
	test_values<float>(float_values);
}

void LexicalCastTests::float_auto()
{
	Value<float> float_values[] =
	{
		{ 0, "%g", "0" },
		{ 1, "%#g", "1.00000" },
		{ 100, "%.3g", "100" },
		{ 1000, "%.3g", "1e+03" },
		{ 1100, "%.3g", "1.1e+03" },
		{ 1e-5, "%g", "1e-05" },
		{ 0.0987, "%g", "0.0987" },
		{ 0.0987, "%.3g", "0.0987" },
		{ 0.0987, "%.2g", "0.099" },
		{ 0, 0, 0 }
	};
	test_values<float>(float_values);
}

void LexicalCastTests::float_sci()
{
	Value<float> float_values[] =
	{
		{ 0, "%e", "0.000000e+00" },
		{ 0.0987, "%e", "9.870000e-02" },
		{ 0.0987, "%.3e", "9.870e-02" },
		{ 0.0987, "%.1e", "9.9e-02" },
		{ 0, 0, 0 }
	};
	test_values<float>(float_values);
}

void LexicalCastTests::float_corner_cases()
{
	Value<float> float_values[] =
	{
		{ 10, "%.20f", "10.00000000000000000000" },
		{ 10, "%.20g", "10" },
		{ 99.99, "%#.3g", "100." },
		{ 999.9, "%.3g", "1e+03" },
		{ 0.09999999, "%.3g", "0.1" },
		{ 0.09999999, "%.3f", "0.100" },
		{ 0, 0, 0 }
	};
	test_values<float>(float_values);
}

template<typename T>
void LexicalCastTests::test_values(const Value<T> *values)
{
	for(const Value<T> *i=values; i->format; ++i)
	{
		string result = lexical_cast<string>(i->value, i->format);
		expect_equal(result, result==i->result, format("result == \"%s\"", c_escape(i->result)));
	}
}
