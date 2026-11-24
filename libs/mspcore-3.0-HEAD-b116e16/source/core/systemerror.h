#ifndef MSP_CORE_SYSTEMERROR_H_
#define MSP_CORE_SYSTEMERROR_H_

#include <stdexcept>
#include <string>

namespace Msp {

class system_error: public std::runtime_error
{
private:
	int code_;

public:
	system_error(const std::string &, int = -1);
	system_error(const std::string &, const std::string &);
	virtual ~system_error() throw() { }

	int code() const throw() { return code_; }

private:
	static std::string get_message(int);
};

} // namespace Msp

#endif
