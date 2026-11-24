#include <windows.h>
#include <msp/strings/lexicalcast.h>
#include "systemerror.h"

using namespace std;

namespace Msp {
string system_error::get_message(int c)
{
	if(c==-1)
		c = GetLastError();

	char msg[1024];
	if(FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, c, 0, msg, sizeof(msg), 0))
		return msg;
	else
		return lexical_cast<string>(c, Fmt().hex());
}

} // namespace Msp
