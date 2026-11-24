#include <windows.h>
#include <msp/stringcodec/utf16.h>
#include <msp/stringcodec/utf8.h>
#include "application.h"

using namespace std;
using namespace Msp;

int main(int argc, char **argv)
{
	return Msp::Application::run(argc, argv);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{ 
	int argc	= 0;
	LPWSTR *argv_w = CommandLineToArgvW(GetCommandLineW(), &argc);

	unsigned max_size = 0;
	for(int i=0; i<argc; ++i)
		max_size += wcslen(argv_w[i])*4+1;

	char *argv_buf = new char[max_size+(argc+1)*sizeof(char *)];
	char **argv = reinterpret_cast<char **>(argv_buf);
	char *argv_ptr = argv_buf+(argc+1)*sizeof(char *);
	for(int i=0; i<argc; ++i)
	{
		argv[i] = argv_ptr;
		string arg(reinterpret_cast<char *>(argv_w[i]), wcslen(argv_w[i])*2);
		arg = StringCodec::transcode<StringCodec::Utf16Le, StringCodec::Utf8>(arg);
		copy(arg.begin(), arg.end(), argv_ptr);
		argv_ptr += arg.size();
		*argv_ptr++ = 0;
	}

	LocalFree(argv_w);
	int exit_code = Msp::Application::run(argc, argv, hInstance);
	delete[] argv_buf;
	return exit_code;
}
