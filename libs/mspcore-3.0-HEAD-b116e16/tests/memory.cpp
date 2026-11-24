#include <msp/io/memory.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class MemoryTests: public Test::RegisteredTest<MemoryTests>
{
public:
	MemoryTests();

	static const char *get_name() { return "Memory"; }

private:
	void write();
	void read();
	void getline();
	void invalid_access();
};


MemoryTests::MemoryTests()
{
	add(&MemoryTests::write,          "write");
	add(&MemoryTests::read,           "read");
	add(&MemoryTests::getline,        "getline");
	add(&MemoryTests::invalid_access, "invalid_access").expect_throw<IO::invalid_access>();
}

void MemoryTests::write()
{
	char buf[64] = { };
	IO::Memory mem(buf, sizeof(buf));
	mem.write("foobar");
	EXPECT(equal(buf, buf+6, "foobar"));
	for(unsigned i=6; i<sizeof(buf); ++i)
		if(buf[i]!=0)
			fail("Garbage in buffer");
}

void MemoryTests::read()
{
	static const char buf[] = "foobar";

	IO::Memory mem(buf, 6);
	char rbuf[16];
	unsigned len = mem.read(rbuf, sizeof(rbuf));
	EXPECT_EQUAL(len, 6);
	EXPECT(equal(rbuf, rbuf+6, buf));
}

void MemoryTests::getline()
{
	static const char buf[] = "foobar\n\nquux\n";

	IO::Memory mem(buf, sizeof(buf)-1);
	string line;
	EXPECT(mem.getline(line));
	EXPECT_EQUAL(line, "foobar");
	EXPECT(mem.getline(line));
	EXPECT_EQUAL(line, "");
	EXPECT(mem.getline(line));
	EXPECT_EQUAL(line, "quux");
	EXPECT(!mem.getline(line));
}

void MemoryTests::invalid_access()
{
	static const char buf[] = "foobar";

	IO::Memory mem(buf, sizeof(buf));
	mem.write("quux", 4);
}
