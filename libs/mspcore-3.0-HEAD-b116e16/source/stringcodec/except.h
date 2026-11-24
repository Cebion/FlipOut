#ifndef MSP_STRINGCODEC_EXCEPT_H_
#define MSP_STRINGCODEC_EXCEPT_H_

#include <stdexcept>
#include "ustring.h"

namespace Msp {
namespace StringCodec {

/**
Base class for codec errors.
*/
class codec_error: public std::runtime_error
{
public:
	codec_error(const std::string &w): std::runtime_error(w) { }
	virtual ~codec_error() throw() { }
};


/**
Thrown when a codec can't encode the requested character.
*/
class invalid_character: public codec_error
{
public:
	invalid_character(unichar, const std::string &);
	virtual ~invalid_character() throw() { }
};


/**
Thrown when a codec encounters a byte sequence it can't decode.
*/
class invalid_sequence: public codec_error
{
public:
	invalid_sequence(const std::string::const_iterator &, const std::string::const_iterator &, const std::string &);
	virtual ~invalid_sequence() throw() { }

private:
	std::string format_sequence(const std::string::const_iterator &, const std::string::const_iterator &);
};

} // namespace StringCodec
} // namespace Msp

#endif
