#include <msp/core/thread.h>
#include <msp/io/pipe.h>
#include <msp/io/poll.h>
#include <msp/time/timedelta.h>
#include <msp/test/test.h>

using namespace Msp;

class PipeTests: public Test::RegisteredTest<PipeTests>
{
public:
	PipeTests();

	static const char *get_name() { return "Pipe"; }

private:
	void readwrite();
	void poll();
};


class PipeWriter: public Thread
{
private:
	IO::Pipe &pipe;

public:
	PipeWriter(IO::Pipe &);

private:
	virtual void main();
};


PipeTests::PipeTests()
{
	add(&PipeTests::readwrite, "Read/write");
	add(&PipeTests::poll, "Poll");
}

void PipeTests::readwrite()
{
	IO::Pipe pipe;
	PipeWriter writer(pipe);

	for(unsigned i=0; i<256; ++i)
	{
		unsigned char c = pipe.get();
		if(c!=i)
			fail("Invalid data");
	}

	writer.join();
}

void PipeTests::poll()
{
	IO::Pipe pipe;

	pipe.put(1);
	IO::PollEvent ev = IO::poll(pipe, IO::P_INPUT);
	EXPECT_EQUAL(ev, IO::P_INPUT);

	pipe.get();

	ev = IO::poll(pipe, IO::P_INPUT, 100*Time::msec);
	EXPECT_EQUAL(ev, IO::P_NONE);
}


PipeWriter::PipeWriter(IO::Pipe &p):
	pipe(p)
{
	launch();
}

void PipeWriter::main()
{
	for(unsigned i=0; i<256; ++i)
		pipe.put(i);
}
