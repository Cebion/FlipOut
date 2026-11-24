#include <msp/fs/path.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class PathTests: public Test::RegisteredTest<PathTests>
{
public:
	PathTests();

	static const char *get_name() { return "Path"; }

private:
	void empty();
	void normalization();
	void concatenation();
	void iterator_abs();
	void iterator_rel();
	void iterator_special();
	void invalid_iterator();
	void indexing();
	void invalid_index();
};

PathTests::PathTests()
{
	add(&PathTests::empty, "Emptiness");
	add(&PathTests::normalization, "Normalization");
	add(&PathTests::concatenation, "Concatenation");
	add(&PathTests::iterator_abs, "Iterator (absolute path)");
	add(&PathTests::iterator_rel, "Iterator (relative path)");
	add(&PathTests::iterator_special, "Iterator (special cases)");
	add(&PathTests::indexing, "Indexing");
	add(&PathTests::invalid_index, "Invalid index").expect_throw<invalid_argument>();
}

void PathTests::empty()
{
	EXPECT(FS::Path().empty());
	EXPECT(FS::Path("").empty());
	EXPECT(!FS::Path(".").empty());
}

void PathTests::normalization()
{
	FS::Path path("foo");
	EXPECT_EQUAL(path.str(), "./foo");
	path = "foo/";
	EXPECT_EQUAL(path.str(), "./foo");
	path = "foo/.";
	EXPECT_EQUAL(path.str(), "./foo");
	path = "foo/..";
	EXPECT_EQUAL(path.str(), ".");
	path = "//foo";
	EXPECT_EQUAL(path.str(), "/foo");
	path = "/..";
	EXPECT_EQUAL(path.str(), "/");
}

void PathTests::concatenation()
{
	FS::Path path(".");
	path /= "foo";
	EXPECT_EQUAL(path.str(), "./foo");
	path /= ".";
	EXPECT_EQUAL(path.str(), "./foo");
	path /= "bar";
	EXPECT_EQUAL(path.str(), "./foo/bar");
	path /= "../../quux";
	EXPECT_EQUAL(path.str(), "./quux");
	path /= "/";
	EXPECT_EQUAL(path.str(), "/");
	path /= "..";
	EXPECT_EQUAL(path.str(), "/");
}

void PathTests::iterator_abs()
{
	FS::Path path("/foo/bar");
	FS::Path::Iterator iter = path.begin();
	EXPECT(iter==path.begin());
	EXPECT_EQUAL(*iter, "/");
	++iter;
	EXPECT_EQUAL(*iter, "foo");
	++iter;
	EXPECT_EQUAL(*iter, "bar");
	++iter;
	EXPECT(iter==path.end());
}

void PathTests::iterator_rel()
{
	FS::Path path("./foo");
	FS::Path::Iterator iter = path.begin();
	EXPECT(iter==path.begin());
	EXPECT_EQUAL(*iter, ".");
	++iter;
	EXPECT_EQUAL(*iter, "foo");
	++iter;
	EXPECT(iter==path.end());
}

void PathTests::iterator_special()
{
	FS::Path path;
	EXPECT(path.begin()==path.end());

	path = ".";
	FS::Path::Iterator iter = path.begin();
	EXPECT_EQUAL(*iter, ".");
	++iter;
	EXPECT(iter==path.end());

	path = "/";
	iter = path.begin();
	EXPECT_EQUAL(*iter, "/");
	++iter;
	EXPECT(iter==path.end());
}

void PathTests::indexing()
{
	FS::Path path("/foo/bar");
	EXPECT_EQUAL(path.size(), 3);
	EXPECT_EQUAL(path[0], "/");
	EXPECT_EQUAL(path[1], "foo");
	EXPECT_EQUAL(path[-1], "bar");
}

void PathTests::invalid_index()
{
	FS::Path path("foo");
	path[3];
}
