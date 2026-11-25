/* $Id$ */
#include <iostream>
#include <string>
#include <msp/core/getopt.h>
#include <msp/strings/regex.h>

using namespace std;
using namespace Msp;

int main(int argc, char **argv)
{
	bool debug=false;
	GetOpt getopt;
	getopt.add_option('d', "debug", debug, GetOpt::NO_ARG);
	getopt(argc, argv);

	const vector<string> &args=getopt.get_args();

	if(args.empty())
	{
		cerr<<"Usage: "<<argv[0]<<" <regex>\n";
		return 1;
	}

	Regex regex(args[0]);
	if(debug)
		cout<<regex.disassemble();
	string line;
	while(getline(cin, line))
	{
		if(RegMatch match=regex.match(line))
			cout<<line<<'\n';
	}

	return 0;
}
