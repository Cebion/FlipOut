#include <map>
#include <msp/core/maputils.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class MapUtilTests: public Test::RegisteredTest<MapUtilTests>
{
public:
	MapUtilTests();

	static const char *get_name() { return "maputils"; }

private:
	void get_item();
	void insert_unique();
	void nonexistent_item();
	void key_collision();
	void uncastable_key();
};


MapUtilTests::MapUtilTests()
{
	add(&MapUtilTests::get_item, "get_item");
	add(&MapUtilTests::insert_unique, "insert_unique");
	add(&MapUtilTests::nonexistent_item, "nonexistent item").expect_throw<key_error>();
	add(&MapUtilTests::key_collision, "key collision").expect_throw<key_error>();
	add(&MapUtilTests::uncastable_key, "uncastable key").expect_throw<key_error>();
}

void MapUtilTests::get_item()
{
	map<int, string> m;
	m[1] = "foo";
	m[3] = "bar";
	EXPECT_EQUAL(::get_item(m, 1), "foo");
	EXPECT_EQUAL(::get_item(m, 3), "bar");
}

void MapUtilTests::insert_unique()
{
	map<int, string> m;
	::insert_unique(m, 1, "foo");
	::insert_unique(m, 3, "bar");
	EXPECT_EQUAL(m.count(1), 1);
	EXPECT_EQUAL(m.count(3), 1);
}

void MapUtilTests::nonexistent_item()
{
	map<int, string> m;
	::get_item(m, 0);
}

void MapUtilTests::key_collision()
{
	map<int, string> m;
	::insert_unique(m, 1, "foo");
	::insert_unique(m, 1, "bar");
}

void MapUtilTests::uncastable_key()
{
	map<pair<int, int>, string> m;
	::get_item(m, make_pair(0, 0));
}
