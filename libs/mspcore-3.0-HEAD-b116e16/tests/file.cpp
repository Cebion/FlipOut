#include <msp/core/systemerror.h>
#include <msp/io/file.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class FileTests: public Test::RegisteredTest<FileTests>
{
public:
	FileTests();

	static const char *get_name() { return "File"; }

private:
	void write();
	void read();
	void seek();
	void file_not_found();
	void invalid_access();
};


FileTests::FileTests()
{
	add(&FileTests::write,          "Writing");
	add(&FileTests::read,           "Reading");
	add(&FileTests::seek,           "Seeking");
	add(&FileTests::file_not_found, "file_not_found").expect_throw<IO::file_not_found>();
	add(&FileTests::invalid_access, "invalid_access").expect_throw<IO::invalid_access>();
}

void FileTests::write()
{
	IO::File out("test.txt", IO::M_WRITE);
	out.write("foobar\n");
	out.write("quux\n");
}

void FileTests::read()
{
	IO::File in("test.txt");

	string line;
	EXPECT(in.getline(line));
	EXPECT_EQUAL(line, "foobar");

	char buf[128];
	unsigned len = in.read(buf, sizeof(buf));
	EXPECT_EQUAL(len, 5U);
	EXPECT_EQUAL(string(buf, len), "quux\n");

	len = in.read(buf, sizeof(buf));
	EXPECT_EQUAL(len, 0U);
	EXPECT(in.eof());
}

void FileTests::seek()
{
	IO::File in("test.txt");

	IO::SeekOffset pos = in.seek(3, IO::S_BEG);
	EXPECT_EQUAL(pos, 3);
	char c = in.get();
	EXPECT_EQUAL(c, 'b');

	pos = in.seek(1, IO::S_CUR);
	EXPECT_EQUAL(pos, 5);
	c = in.get();
	EXPECT_EQUAL(c, 'r');

	pos = in.seek(-2, IO::S_END);
	EXPECT_EQUAL(pos, 10);
	c = in.get();
	EXPECT_EQUAL(c, 'x');
}

void FileTests::file_not_found()
{
	IO::File in("does.not.exist");
}

void FileTests::invalid_access()
{
	IO::File in("test.txt");
	in.write("foo\n");
}
