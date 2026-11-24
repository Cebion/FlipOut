#include "asset.h"

using namespace std;

namespace Msp {
namespace IO {

void Asset::set_block(bool)
{
	throw logic_error("Asset::set_block");
}

void Asset::set_inherit(bool)
{
	throw logic_error("Asset::set_inherit");
}

unsigned Asset::do_write(const char *, unsigned)
{
	throw invalid_access(M_WRITE);
}

const Handle &Asset::get_handle(Mode)
{
	throw logic_error("Asset::get_handle");
}

} // namespace IO
} // namespace Msp
