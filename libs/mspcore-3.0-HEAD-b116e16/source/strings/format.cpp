#include "format.h"

using namespace std;

namespace Msp {

Formatter::Formatter(const string &f):
	fmt(f),
	pos(fmt.begin())
{
	advance();
}

Formatter::Formatter(const Formatter &other):
	fmt(other.fmt),
	pos(fmt.begin()+(other.pos-other.fmt.begin())),
	result(other.result)
{ }

Formatter &Formatter::operator=(const Formatter &other)
{
	fmt = other.fmt;
	pos = fmt.begin()+(other.pos-other.fmt.begin());
	result = other.result;
	return *this;
}

const string &Formatter::str() const
{
	if(pos!=fmt.end())
		throw format_error("Too few arguments for format");

	return result;
}

void Formatter::advance()
{
	for(; pos!=fmt.end(); ++pos)
	{
		if(*pos=='%')
		{
			++pos;
			if(pos==fmt.end())
				throw format_error("Malformed format string");
			if(*pos!='%')
				break;
		}

		result += *pos;
	}
}

Fmt Formatter::get_conversion()
{
	if(pos==fmt.end())
		throw format_error("Too many arguments for format");

	auto i = pos;
	for(; i!=fmt.end(); ++i)
		if(isalpha(*i))
			break;

	if(i==fmt.end())
		throw format_error("Malformed format string");

	++i;
	string c(pos, i);
	pos = i;

	return Fmt(c);
}

} // namespace Msp
