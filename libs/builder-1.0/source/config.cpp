/* $Id: config.cpp 97 2009-09-20 16:19:05Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstdlib>
#include <msp/core/except.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/except.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include <msp/time/utils.h>
#include "builder.h"
#include "config.h"
#include "sourcepackage.h"

using namespace std;
using namespace Msp;

Config::Config(SourcePackage &p):
	package(p),
	freeze_mtime(false)
{ }

void Config::add_option(const string &n, const string &v, const string &d)
{
	options.insert(OptionMap::value_type(n, Option(n, v, d)));
}

const Config::Option &Config::get_option(const string &name) const
{
	OptionMap::const_iterator i=options.find(name);
	if(i==options.end())
		throw KeyError("Unknown option", name);

	return i->second;
}

bool Config::is_option(const string &name) const
{
	return options.count(name);
}

void Config::select_last_profile()
{
	try
	{
		IO::BufferedFile in((package.get_source()/".profile").str());
		string profile;
		in.getline(profile);
		set_option("profile", profile);
	}
	catch(const IO::FileNotFound &)
	{ }

	freeze_mtime=true;
	package.get_builder().apply_profile_template(*this, get_option("profile").value);
	freeze_mtime=false;

	load();
}

void Config::select_profile(const string &profile)
{
	set_option("profile", profile);

	if(!package.get_builder().get_dry_run())
	{
		IO::BufferedFile out((package.get_source()/".profile").str(), IO::M_WRITE);
		IO::print(out, "%s\n", profile);
	}

	freeze_mtime=true;
	package.get_builder().apply_profile_template(*this, profile);
	freeze_mtime=false;

	load();
}

bool Config::update(const StringMap &opts)
{
	bool changed=false;
	for(StringMap::const_iterator i=opts.begin(); i!=opts.end(); ++i)
	{
		if(set_option(i->first, i->second) && i->first!="profile")
			changed=true;
	}

	if(changed && !freeze_mtime)
		mtime=Time::now();

	return changed;
}

void Config::finish()
{
	for(unsigned n=0; n<20; ++n)
	{
		bool changed=false;
		for(OptionMap::iterator i=options.begin(); i!=options.end(); ++i)
		{
			Option &opt=i->second;
			string::size_type dollar=0;
			while((dollar=opt.value.find('$', dollar))!=string::npos)
			{
				string::size_type end;
				string var;
				if(opt.value[dollar+1]=='{')
				{
					end=opt.value.find('}', dollar+2);
					if(end==string::npos)
						throw Exception("Unterminated variable reference");
					var=opt.value.substr(dollar+2, end-dollar-2);
					++end;
				}
				else
				{
					for(end=dollar+1; (isalnum(opt.value[end]) && opt.value[end]!='_'); ++end) ;
					var=opt.value.substr(dollar+1, end-dollar-1);
				}

				string value;
				if(is_option(var))
					value=get_option(var).value;
				else if(var=="arch")
					value=package.get_builder().get_current_arch().get_name();
				else if(const char *ptr=getenv(var.c_str()))
					value=ptr;

				opt.value.replace(dollar, end-dollar, value);

				dollar+=value.size();
				changed=true;
			}
		}

		if(!changed)
			break;
	}
}

void Config::save() const
{
	FS::Path fn=package.get_source()/".options";

	OptionMap::const_iterator i=options.find("profile");
	if(i!=options.end())
		fn=package.get_source()/(".options."+i->second.value);

	IO::BufferedFile out(fn.str(), IO::M_WRITE);

	for(i=options.begin(); i!=options.end(); ++i)
		IO::print(out, "option \"%s\" \"%s\";\n", i->second.name, i->second.value);
}

bool Config::set_option(const string &opt, const string &val)
{
	bool result=false;

	OptionMap::iterator i=options.find(opt);
	if(i!=options.end())
	{
		if(i->second.value!=val)
			result=true;
		i->second.value=val;
	}

	return result;
}

void Config::load()
{
	FS::Path fn=package.get_source()/(".options."+get_option("profile").value);

	try
	{
		IO::BufferedFile in(fn.str());

		mtime=Time::TimeStamp::from_unixtime(FS::stat(fn).st_mtime);

		DataFile::Parser parser(in, fn.str());
		Loader loader(*this);
		loader.load(parser);
	}
	catch(const IO::FileNotFound &)
	{ }
}


Config::Option::Option(const string &n, const string &v, const string &d):
	name(n),
	defv(v),
	descr(d),
	value(v)
{ }


Config::Loader::Loader(Config &c):
	conf(c)
{
	add("option", &Loader::option);
}

void Config::Loader::option(const string &n, const string &v)
{
	conf.set_option(n, v);
}
