/* $Id: getopt.cpp 68 2009-09-20 16:16:11Z tdb $

This file is part of libmspcore
Copyright © 2006-2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#include "getopt.h"

using namespace std;

namespace Msp {

GetOpt::~GetOpt()
{
	for(list<OptBase *>::iterator i=opts.begin(); i!=opts.end(); ++i)
		delete *i;
}

/**
Generates a single line that gives an overview about the known options.

@param   argv0  The program name to be used in the usage string

@return  The generated usage string
*/
string GetOpt::generate_usage(const string &argv0) const
{
	ostringstream line;
	
	line<<argv0;
	for(list<OptBase *>::const_iterator i=opts.begin(); i!=opts.end(); ++i)
	{
		line<<" [";
		if((*i)->get_short())
		{
			line<<'-'<<(*i)->get_short();
			if(!(*i)->get_long().empty())
				line<<'|';
			else if((*i)->get_arg_type()==OPTIONAL_ARG)
				line<<'['<<(*i)->get_metavar()<<']';
			else if((*i)->get_arg_type()==REQUIRED_ARG)
				line<<' '<<(*i)->get_metavar();
		}
		if(!(*i)->get_long().empty())
		{
			line<<"--"<<(*i)->get_long();

			if((*i)->get_arg_type()==OPTIONAL_ARG)
				line<<"[="<<(*i)->get_metavar()<<']';
			else if((*i)->get_arg_type()==REQUIRED_ARG)
				line<<'='<<(*i)->get_metavar();
		}
		line<<']';
	}

	return line.str();
}

/**
Generates help for known options in tabular format, one option per line.
The returned string will have a linefeed at the end.
*/
string GetOpt::generate_help() const
{
	bool any_short=false;
	for(list<OptBase *>::const_iterator i=opts.begin(); (!any_short && i!=opts.end()); ++i)
		any_short=(*i)->get_short();

	string::size_type maxw=0;
	list<string> switches;
	for(list<OptBase *>::const_iterator i=opts.begin(); i!=opts.end(); ++i)
	{
		ostringstream swtch;
		if((*i)->get_short())
		{
			swtch<<'-'<<(*i)->get_short();
			if(!(*i)->get_long().empty())
				swtch<<", ";
			else if((*i)->get_arg_type()==OPTIONAL_ARG)
				swtch<<'['<<(*i)->get_metavar()<<']';
			else if((*i)->get_arg_type()==REQUIRED_ARG)
				swtch<<' '<<(*i)->get_metavar();
		}
		else if(any_short)
			swtch<<"    ";
		if(!(*i)->get_long().empty())
		{
			swtch<<"--"<<(*i)->get_long();

			if((*i)->get_arg_type()==OPTIONAL_ARG)
				swtch<<"[="<<(*i)->get_metavar()<<']';
			else if((*i)->get_arg_type()==REQUIRED_ARG)
				swtch<<'='<<(*i)->get_metavar();
		}
		switches.push_back(swtch.str());
		maxw=max(maxw, switches.back().size());
	}

	string result;
	list<string>::const_iterator j=switches.begin();
	for(list<OptBase *>::const_iterator i=opts.begin(); i!=opts.end(); ++i, ++j)
	{
		result+="  "+*j;
		result+=string(maxw+2-j->size(), ' ');
		result+=(*i)->get_help();
		result+='\n';
	}
	
	return result;
}

void GetOpt::operator()(unsigned argc, const char *const *argv)
{
	unsigned i=1;
	for(; i<argc;)
	{
		if(argv[i][0]=='-')
		{
			if(argv[i][1]=='-')
			{
				if(!argv[i][2])
					break;

				i+=process_long(argv+i);
			}
			else
				i+=process_short(argv+i);
		}
		else
			args.push_back(argv[i++]);
	}
	
	for(; i<argc; ++i)
		args.push_back(argv[i]);
}

GetOpt::OptBase &GetOpt::get_option(char s)
{
	for(list<OptBase *>::iterator i=opts.begin(); i!=opts.end(); ++i)
		if((*i)->get_short()==s)
			return **i;
	throw UsageError(string("Unknown option -")+s);
}

GetOpt::OptBase &GetOpt::get_option(const string &l)
{
	for(list<OptBase *>::iterator i=opts.begin(); i!=opts.end(); ++i)
		if((*i)->get_long()==l)
			return **i;
	throw UsageError(string("Unknown option --")+l);
}

/**
Processes the given argument as a long option.

@param   argp  Pointer to the argument

@return  The number of arguments eaten (1 or 2)
*/
unsigned GetOpt::process_long(const char *const *argp)
{
	// Skip the --
	const char *arg=argp[0]+2;

	// See if the argument contains an =
	unsigned equals=0;
	for(; arg[equals] && arg[equals]!='='; ++equals) ;
	
	OptBase &opt=get_option(string(arg, equals));
	
	if(arg[equals])
		// Process the part after the = as option argument
		opt.process(arg+equals+1);
	else if(opt.get_arg_type()==REQUIRED_ARG)
	{
		if(!argp[1])
			throw UsageError("Premature end of arguments");

		// Process the next argument as option argument
		opt.process(argp[1]);
		return 2;
	}
	else
		opt.process();
	
	return 1;
}

/**
Processes short options from the given argument.

@param   argp  Pointer to the argument

@return  The number of arguments eaten (1 or 2)
*/
unsigned GetOpt::process_short(const char *const *argp)
{
	// Skip the -
	const char *arg=argp[0]+1;

	// Loop through all characters in the argument
	for(; *arg; ++arg)
	{
		OptBase &opt=get_option(*arg);

		if(arg[1] && opt.get_arg_type()!=NO_ARG)
		{
			// Need an option argument and we have characters left - use them
			opt.process(arg+1);
			return 1;
		}
		else if(opt.get_arg_type()==REQUIRED_ARG)
		{
			if(!argp[1])
				throw UsageError("Premature end of arguments");
			
			// Use the next argument as option argument
			opt.process(argp[1]);
			return 2;
		}
		else
			opt.process();
	}

	return 1;
}


GetOpt::OptBase::OptBase(char s, const std::string &l, ArgType a):
	shrt(s),
	lng(l),
	arg_type(a),
	seen_count(0),
	metavar("ARG")
{ }

GetOpt::OptBase &GetOpt::OptBase::set_help(const string &h)
{
	help=h;
	return *this;
}

GetOpt::OptBase &GetOpt::OptBase::set_help(const string &h, const string &m)
{
	help=h;
	metavar=m;
	return *this;
}

void GetOpt::OptBase::process()
{
	if(arg_type==REQUIRED_ARG)
		throw UsageError("--"+lng+" requires an argument");
	++seen_count;

	store();
}

void GetOpt::OptBase::process(const string &arg)
{
	if(arg_type==NO_ARG)
		throw UsageError("--"+lng+" takes no argument");
	++seen_count;

	store(arg);
}

} // namespace Msp
