#include <msp/core/application.h>
#include <msp/core/getopt.h>
#include <msp/io/console.h>
#include <msp/io/file.h>
#include <msp/io/zlibcompressed.h>

using namespace std;
using namespace Msp;

class Z: public RegisteredApplication<Z>
{
private:
	IO::ZlibCompressed *zlib;
	IO::File *input_file;
	IO::Base *input;
	IO::Base *output;
	bool decompress;

public:
	Z(int, char **);
	~Z();

	virtual int main();
};

Z::Z(int argc, char **argv):
	input_file(0)
{
	string input_fn;
	GetOpt getopt;
	getopt.add_option('d', "decompress", decompress, GetOpt::NO_ARG);
	getopt.add_argument("filename", input_fn, GetOpt::OPTIONAL_ARG);
	getopt(argc, argv);

	if(!input_fn.empty())
		input_file = new IO::File(input_fn);

	input = (input_file ? static_cast<IO::Base *>(input_file) : static_cast<IO::Base *>(&IO::cin));
	output = &IO::cout;
	if(decompress)
	{
		zlib = new IO::ZlibCompressed(*input);
		input = zlib;
	}
	else
	{
		zlib = new IO::ZlibCompressed(*output);
		output = zlib;
	}
}

Z::~Z()
{
	delete zlib;
	delete input_file;
}

int Z::main()
{
	char buffer[1024];
	while(1)
	{
		unsigned len = input->read(buffer, sizeof(buffer));
		if(!len)
			break;
		output->write(buffer, len);
	}
	return 0;
}
