#include <iostream>
#include <string>
#include <msp/core/getopt.h>
#include <msp/strings/regex.h>

using namespace std;
using namespace Msp;

int main(int argc, char **argv)
{
	bool debug = false;
	string re_str;
	GetOpt getopt;
	getopt.add_option('d', "debug", debug, GetOpt::NO_ARG);
	getopt.add_argument("regex", re_str, GetOpt::REQUIRED_ARG);
	getopt(argc, argv);

	Regex regex(re_str);
	if(debug)
		cout << regex.disassemble();
	string line;
	while(getline(cin, line))
	{
		if(RegMatch match = regex.match(line))
			cout << line << '\n';
	}

	return 0;
}
