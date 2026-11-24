#include <msp/strings/format.h>
#include <msp/strings/utils.h>
#include "except.h"

using namespace std;

namespace Msp {
namespace StringCodec {

invalid_character::invalid_character(unichar ch, const string &detail):
	codec_error(format("U+%04X (%s)", ch, detail))
{ }


invalid_sequence::invalid_sequence(const string::const_iterator &begin, const string::const_iterator &end, const string &detail):
	codec_error(format("%s (%s)", format_sequence(begin, end), detail))
{ }

string invalid_sequence::format_sequence(const string::const_iterator &begin, const string::const_iterator &end)
{
	string result;
	for(auto i=begin; i!=end; ++i)
		append(result, " ", lexical_cast<string>(static_cast<unsigned char>(*i), Fmt().fill('0').width(2).hex().uppercase()));
	return result;
}

} // namespace StringCodec
} // namespace Msp
