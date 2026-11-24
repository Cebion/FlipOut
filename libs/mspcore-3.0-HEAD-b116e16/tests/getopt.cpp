#include <msp/core/getopt.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class GetOptTests: public Test::RegisteredTest<GetOptTests>
{
public:
	GetOptTests();

	static const char *get_name() { return "GetOpt"; }

private:
	void short_options();
	void long_options();
	void arguments();
	void positional();
	void positional_list();
	void empty_list();
	void mixed();
	void help();
	void invalid_option();
	void invalid_arg();
	void missing_arg();
	void missing_positional();
	void missing_positional_list();
};

GetOptTests::GetOptTests()
{
	add(&GetOptTests::short_options, "Short options");
	add(&GetOptTests::long_options, "Long options");
	add(&GetOptTests::arguments, "Option arguments");
	add(&GetOptTests::positional, "Positional arguments");
	add(&GetOptTests::positional_list, "Positional argument list");
	add(&GetOptTests::empty_list, "Empty positional argument list");
	add(&GetOptTests::mixed, "Mixed options and positional arguments");
	add(&GetOptTests::help, "Help").expect_throw<usage_error>();
	add(&GetOptTests::invalid_option, "Invalid option").expect_throw<usage_error>();
	add(&GetOptTests::invalid_arg, "Invalid option argument").expect_throw<usage_error>();
	add(&GetOptTests::missing_arg, "Missing option argument").expect_throw<usage_error>();
	add(&GetOptTests::missing_positional, "Missing positional argument").expect_throw<usage_error>();
	add(&GetOptTests::missing_positional_list, "Missing positional argument list").expect_throw<usage_error>();
}

void GetOptTests::short_options()
{
	static const char *argv[] = { "test", "-a", "-bc", 0 };

	bool a = false;
	bool b = false;
	bool c = false;

	GetOpt getopt;
	getopt.add_option('a', "a", a, GetOpt::NO_ARG);
	getopt.add_option('b', "b", b, GetOpt::NO_ARG);
	getopt.add_option('c', "c", c, GetOpt::NO_ARG);
	getopt(3, argv);

	EXPECT(a && b && c);
}

void GetOptTests::long_options()
{
	static const char *argv[] = { "test", "--foo", "--bar", 0 };

	bool foo = false;
	bool bar = false;

	GetOpt getopt;
	getopt.add_option("foo", foo, GetOpt::NO_ARG);
	getopt.add_option("bar", bar, GetOpt::NO_ARG);
	getopt(3, argv);

	EXPECT(foo && bar);
}

void GetOptTests::arguments()
{
	static const char *argv[] = { "test", "-aabc", "-b", "x y z", "--foo=42", "--bar", "69", 0 };

	string a;
	string b;
	int foo = 0;
	int bar = 0;

	GetOpt getopt;
	getopt.add_option('a', "a", a, GetOpt::REQUIRED_ARG);
	getopt.add_option('b', "b", b, GetOpt::REQUIRED_ARG);
	getopt.add_option("foo", foo, GetOpt::REQUIRED_ARG);
	getopt.add_option("bar", bar, GetOpt::REQUIRED_ARG);
	getopt(7, argv);

	EXPECT_EQUAL(a, "abc");
	EXPECT_EQUAL(b, "x y z");
	EXPECT_EQUAL(foo, 42);
	EXPECT_EQUAL(bar, 69);
}

void GetOptTests::positional()
{
	static const char *argv[] = { "test", "foo", "42", "bar", 0 };

	string first;
	unsigned second = 0;
	string third;
	unsigned fourth = 13;

	GetOpt getopt;
	getopt.add_argument("first", first);
	getopt.add_argument("second", second);
	getopt.add_argument("third", third);
	getopt.add_argument("fourth", fourth, GetOpt::OPTIONAL_ARG);
	getopt(4, argv);

	EXPECT_EQUAL(first, "foo");
	EXPECT_EQUAL(second, 42);
	EXPECT_EQUAL(third, "bar");
	EXPECT_EQUAL(fourth, 13);
}

void GetOptTests::positional_list()
{
	static const char *argv[] = { "test", "x", "y", "z", "foo", 0 };

	list<string> args;
	string tail;

	GetOpt getopt;
	getopt.add_argument("arg", args);
	getopt.add_argument("tail", tail);
	getopt(5, argv);

	EXPECT_EQUAL(args.size(), 3U);
	EXPECT_EQUAL(args.front(), "x");
	EXPECT_EQUAL(args.back(), "z");
	EXPECT_EQUAL(tail, "foo");
}

void GetOptTests::empty_list()
{
	static const char *argv[] = { "test", "foo", "bar", 0 };

	string head;
	list<string> mid;
	string tail;

	GetOpt getopt;
	getopt.add_argument("head", head);
	getopt.add_argument("mid", mid, GetOpt::OPTIONAL_ARG);
	getopt.add_argument("tail", tail);
	getopt(3, argv);

	EXPECT_EQUAL(head, "foo");
	EXPECT(mid.empty());
	EXPECT_EQUAL(tail, "bar");
}

void GetOptTests::mixed()
{
	static const char *argv[] = { "test", "-a", "foo", "-b", "bar", "baz", 0 };

	bool a = false;
	bool b = false;
	string first;
	string second;
	string third;

	GetOpt getopt;
	getopt.add_option('a', "a", a, GetOpt::NO_ARG);
	getopt.add_option('b', "b", b, GetOpt::NO_ARG);
	getopt.add_argument("first", first);
	getopt.add_argument("second", second);
	getopt.add_argument("third", third);
	getopt(6, argv);

	EXPECT(a && b);
	EXPECT_EQUAL(first, "foo");
	EXPECT_EQUAL(second, "bar");
	EXPECT_EQUAL(third, "baz");
}

void GetOptTests::help()
{
	static const char *argv[] = { "test", "--help", 0 };

	GetOpt getopt;
	getopt(2, argv);
}

void GetOptTests::invalid_option()
{
	static const char *argv[] = { "test", "--invalid", 0 };

	GetOpt getopt;
	getopt(2, argv);
}

void GetOptTests::invalid_arg()
{
	static const char *argv[] = { "test", "--intval=foo", 0 };

	int value;
	GetOpt getopt;
	getopt.add_option("intval", value, GetOpt::REQUIRED_ARG);
	getopt(2, argv);
}

void GetOptTests::missing_arg()
{
	static const char *argv[] = { "test", "--value", 0 };

	string value;
	GetOpt getopt;
	getopt.add_option("value", value, GetOpt::REQUIRED_ARG);
	getopt(2, argv);
}

void GetOptTests::missing_positional()
{
	static const char *argv[] = { "test", 0 };

	string arg;

	GetOpt getopt;
	getopt.add_argument("arg", arg);
	getopt(1, argv);
}

void GetOptTests::missing_positional_list()
{
	static const char *argv[] = { "test", 0 };

	list<string> args;

	GetOpt getopt;
	getopt.add_argument("arg", args);
	getopt(1, argv);
}
