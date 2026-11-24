#include <msp/core/variant.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class VariantTests: public Test::RegisteredTest<VariantTests>
{
public:
	VariantTests();

	static const char *get_name() { return "Variant"; }

private:
	void integer();
	void stdstring();
	void pointer();
	void ref_into();
	void copying();
	void destruction();
	void types();
	void mismatch();
};

class Counter
{
private:
	int &count;

public:
	Counter(int &c): count(c) { ++count; }
	Counter(const Counter &c): count(c.count) { ++count; }
	~Counter() { --count; }
};

VariantTests::VariantTests()
{
	add(&VariantTests::integer, "Integer");
	add(&VariantTests::stdstring, "std::string");
	add(&VariantTests::pointer, "Pointer");
	add(&VariantTests::ref_into, "Reference into");
	add(&VariantTests::copying, "Copying");
	add(&VariantTests::destruction, "Destruction");
	add(&VariantTests::types, "Type checks");
	add(&VariantTests::mismatch, "Mismatch").expect_throw<type_mismatch>();
}

void VariantTests::integer()
{
	Variant var = 42;
	EXPECT_EQUAL(var.value<int>(), 42);
}

void VariantTests::stdstring()
{
	Variant var = string("foobar");
	EXPECT_EQUAL(var.value<string>(), "foobar");
}

void VariantTests::pointer()
{
	int i = 7;
	Variant var = &i;
	*var.value<int *>() = 42;
	EXPECT_EQUAL(i, 42);
}

void VariantTests::ref_into()
{
	Variant var = 7;
	int &r = var.value<int>();
	r = 42;
	EXPECT_EQUAL(var.value<int>(), 42);
}

void VariantTests::copying()
{
	Variant var = 42;
	Variant var2 = var;
	EXPECT_EQUAL(var2.value<int>(), 42);
}

void VariantTests::destruction()
{
	int count = 0;
	{
		Variant var = Counter(count);
		Variant var2 = var;
		EXPECT_EQUAL(count, 2);
		Variant var3;
		var3 = var;
		EXPECT_EQUAL(count, 3);
		var2 = Variant();
		EXPECT_EQUAL(count, 2);
	}
	EXPECT_EQUAL(count, 0);
}

void VariantTests::types()
{
	Variant var = 42U;
	EXPECT(!var.check_type<int>());
	EXPECT(var.check_type<unsigned>());
	EXPECT(!var.check_type<float>());
	EXPECT(!var.check_type<bool>());
	EXPECT(!var.check_type<string>());
}

void VariantTests::mismatch()
{
	Variant var = 1;
	var.value<string>();
}
