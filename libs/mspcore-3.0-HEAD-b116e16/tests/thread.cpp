#include <list>
#include <vector>
#include <msp/core/thread.h>
#include <msp/time/timedelta.h>
#include <msp/time/utils.h>
#include <msp/test/test.h>

using namespace std;
using namespace Msp;

class ThreadTests: public Test::RegisteredTest<ThreadTests>
{
private:
	std::vector<int> data;

public:
	ThreadTests();

	static const char *get_name() { return "Thread"; }

private:
	void single();
	void multiple();
};


class TestThread: public Thread
{
private:
	int *start;
	unsigned count;
	unsigned step;
	int value;
	bool done;

public:
	TestThread(int *, unsigned, unsigned, int);

private:
	virtual void main();

public:
	bool is_done() const { return done; }
};


ThreadTests::ThreadTests():
	data(1000000)
{
	add(&ThreadTests::single, "Single thread");
	add(&ThreadTests::multiple, "Multiple threads");
}

void ThreadTests::single()
{
	fill(data.begin(), data.end(), -1);

	TestThread thread(&data[0], data.size(), 1, 1);
	unsigned wait = 100;
	while(wait && !thread.is_done())
	{
		Time::sleep(100*Time::msec);
		--wait;
	}

	if(!wait)
		fail("Thread did not finish");

	thread.join();

	for(vector<int>::iterator i=data.begin(); i!=data.end(); ++i)
		if(*i!=1)
			fail("Invalid data");
}

void ThreadTests::multiple()
{
	fill(data.begin(), data.end(), -1);

	list<TestThread *> threads;
	for(unsigned i=0; i<10; ++i)
		threads.push_back(new TestThread(&data[i], data.size()/10, 10, i+1));
	unsigned wait = 100;
	while(wait && !threads.empty())
	{
		Time::sleep(100*Time::msec);
		for(list<TestThread *>::iterator i=threads.begin(); i!=threads.end();)
		{
			if((*i)->is_done())
			{
				(*i)->join();
				delete *i;
				threads.erase(i++);
			}
			else
				++i;
		}
		--wait;
	}

	if(!wait)
		fail("Threads did not finish");

	for(unsigned i=0; i<data.size(); ++i)
		if(data[i]!=static_cast<int>(i%10+1))
			fail("Invalid data");
}


TestThread::TestThread(int *s, unsigned c, unsigned t, int v):
	start(s),
	count(c),
	step(t),
	value(v),
	done(false)
{
	launch();
}

void TestThread::main()
{
	int *ptr = start;
	for(unsigned i=0; i<count; ++i)
	{
		*ptr = value;
		ptr += step;
	}
	done = true;
}
