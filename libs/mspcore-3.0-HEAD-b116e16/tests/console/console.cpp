#include <msp/io/console.h>
#include <msp/io/poll.h>
#include <msp/io/print.h>

using namespace Msp;

int main()
{
	IO::cin.set_local_echo(false);
	IO::cin.set_line_buffer(false);
	while(1)
	{
		if(IO::poll(IO::cin, IO::P_INPUT)==IO::P_INPUT)
		{
			char c = IO::cin.get();
			IO::print("Got character '%c' (%02X)\n", c, c);
		}
		else
		{
			IO::print("EOF\n");
			break;
		}
	}

	return 0;
}
