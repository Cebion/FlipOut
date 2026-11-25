/* $Id$

This file is part of libmspio
Copyright © 2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_IO_FILTERED_H_
#define MSP_IO_FILTERED_H_

namespace Msp {
namespace IO {

template<typename B, typename F>
class Filtered: public B
{
private:
	struct Activator
	{
		Filtered &f;

		Activator(Filtered &f_): f(f_) { f.active=true; }
		~Activator() { f.active=false; }
	};

	F filter;
	bool active;

public:
	Filtered(): filter(*this), active(false) { }
	~Filtered() { active=true; }

	template<typename A0>
	Filtered(A0 a0): B(a0), filter(*this), active(false) { }

	template<typename A0, typename A1>
	Filtered(A0 a0, A1 a1): B(a0, a1), filter(*this), active(false) { }

	virtual unsigned put(char c) { return filter.put(c); }
	virtual bool getline(std::string &l) { return filter.getline(l); }
	virtual int get() { return filter.get(); }

	F &get_filter() { return filter; }
protected:
	virtual unsigned do_write(const char *b, unsigned s)
	{
		if(!active)
		{
			Activator a(*this);
			return filter.write(b, s);
		}
		else
			return B::do_write(b, s);
	}

	virtual unsigned do_read(char *b, unsigned s)
	{
		if(!active)
		{
			Activator a(*this);
			return filter.read(b, s);
		}
		else
			return B::do_read(b, s);
	}
};

} // namespace IO
} // namespace Msp

#endif
