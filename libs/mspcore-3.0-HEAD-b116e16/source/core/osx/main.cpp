#include <cstring>
#include "application.h"

int main(int argc, char **argv)
{
	void *data = 0;

	/* Applications launched from Finder get a special argument, which would
	interfere with GetOpt.  Remove it from argv but pass it as data so it can
	be used to determine how the application was launhed. */
	int j = 0;
	for(int i=0; i<argc; ++i)
	{
		if(!strncmp(argv[i], "-psn_", 5))
			data = argv[i];
		else
			argv[j++] = argv[i];
	}
	argc = j;
	argv[j] = 0;

	return Msp::Application::run(argc, argv, data);
}
