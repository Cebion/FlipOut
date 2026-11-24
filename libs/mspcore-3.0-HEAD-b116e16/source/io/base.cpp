#include "base.h"
#include "poll.h"

using namespace std;

namespace Msp {
namespace IO {

Base::Base():
	mode(M_READ),
	eof_flag(false),
	mutex(0)
{ }

Base::~Base()
{
	signal_deleted.emit();
	delete mutex;
}

void Base::check_access(Mode m) const
{
	if(!(mode&m))
		throw invalid_access(m);
}

bool Base::getline(string &line)
{
	line.clear();

	if(eof_flag)
		return false;

	while(!eof())
	{
		int c = get();
		if(c==-1 || c=='\n')
			break;
		line += c;
	}

	return !eof_flag || !line.empty();
}

int Base::get()
{
	char c;
	if(do_read(&c, 1)==0)
		return -1;
	return static_cast<unsigned char>(c);
}

void Base::set_eof()
{
	if(!eof_flag)
	{
		eof_flag = true;
		signal_end_of_file.emit();
	}
}


Base::Synchronize::Synchronize(Base &i):
	io(i)
{
	if(!io.mutex)
		io.mutex = new Mutex;
	io.mutex->lock();
}

Base::Synchronize::~Synchronize()
{
	io.mutex->unlock();
}

} // namespace IO
} // namespace Msp
