#include <msp/fs/utils.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class FSUtilTests: public Test::RegisteredTest<FSUtilTests>
{
public:
	FSUtilTests();

	static const char *get_name() { return "fsutils"; }

private:
	void pathsplit();
	void fnsplit();
	void hierarchy();
	void bad_hierarchy();
};

FSUtilTests::FSUtilTests()
{
	add(&FSUtilTests::pathsplit, "Path splitting");
	add(&FSUtilTests::fnsplit, "Filename splitting");
	add(&FSUtilTests::hierarchy, "Hierarchy");
	add(&FSUtilTests::bad_hierarchy, "Bad hierarchy").expect_throw<invalid_argument>();
}

void FSUtilTests::pathsplit()
{
	FS::Path path("/foo/bar/baz");
	FS::Path dir = FS::dirname(path);
	EXPECT_EQUAL(dir.str(), "/foo/bar");
	string base = FS::basename(path);
	EXPECT_EQUAL(base, "baz");
}

void FSUtilTests::fnsplit()
{
	string filename = "file.png";
	string base = FS::basepart(filename);
	EXPECT_EQUAL(base, "file");
	string ext = FS::extpart(filename);
	EXPECT_EQUAL(ext, ".png");

	filename = "file.tar.gz";
	base = FS::basepart(filename);
	EXPECT_EQUAL(base, "file.tar");
	ext = FS::extpart(filename);
	EXPECT_EQUAL(ext, ".gz");
}

void FSUtilTests::hierarchy()
{
	FS::Path path1 = "/foo/bar";
	FS::Path path2 = "/foo/bar/quux/baz";
	int depth = FS::descendant_depth(path2, path1);
	EXPECT_EQUAL(depth, 2);
	FS::Path rel = FS::relative(path2, path1);
	EXPECT_EQUAL(rel.str(), "./quux/baz");

	path2 = "/foo/quux/baz";
	depth = FS::descendant_depth(path2, path1);
	EXPECT_EQUAL(depth, -1);
	rel = FS::relative(path2, path1);
	EXPECT_EQUAL(rel.str(), "../quux/baz");
	FS::Path anc = FS::common_ancestor(path1, path2);
	EXPECT_EQUAL(anc.str(), "/foo");

	path2 = "bar/quux";
	anc = FS::common_ancestor(path1, path2);
	EXPECT(anc.empty());
}

void FSUtilTests::bad_hierarchy()
{
	FS::Path path1 = "/foo";
	FS::Path path2 = "bar";
	FS::relative(path2, path1);
}
