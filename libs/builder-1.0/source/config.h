/* $Id: config.h 84 2009-02-19 16:43:37Z tdb $

This file is part of builder
Copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <map>
#include <string>
#include <msp/datafile/loader.h>
#include <msp/fs/path.h>
#include <msp/time/timestamp.h>
#include "misc.h"

class SourcePackage;

/**
Manages configuration for a package.  A configuration may have an arbitary
amount of options, as well as a modification time (mtime).
*/
class Config
{
public:
	/** A single configuration option. */
	struct Option
	{
		std::string name;
		std::string defv;
		std::string descr;
		std::string value;

		Option(const std::string &, const std::string &, const std::string &);
	};

	typedef std::map<std::string, Option> OptionMap;

private:
	class Loader: public Msp::DataFile::Loader
	{
	private:
		Config &conf;

	public:
		Loader(Config &);
	private:
		void option(const std::string &, const std::string &);
	};

	SourcePackage &package;
	OptionMap options;
	Msp::Time::TimeStamp mtime;
	bool freeze_mtime;

public:
	Config(SourcePackage &);

	/** Adds a configuration option with name, default value and description. */
	void add_option(const std::string &, const std::string &, const std::string &);

	/** Gets a configuration option by name. */
	const Option &get_option(const std::string &) const;

	const OptionMap &get_options() const { return options; }
	const Msp::Time::TimeStamp &get_mtime() const { return mtime; }

	/** Checks whether an option exists. */
	bool is_option(const std::string &) const;

	/** Selects the last profile used.  If the profile cache file is not
	present, the default profile is assumed. */
	void select_last_profile();

	/** Selects a profile.  The profile cache file is updated, unless doing a
	dry run. */
	void select_profile(const std::string &);

	/** Processes options from the given raw option map.  Nonexistent options
	are ignored.  If any options were changed, the mtime of the configuration is
	updated to the current time.  Return value indicates whether any options
	were changed. */
	bool update(const StringMap &);

	/** Expands any variable references in options. */
	void finish();

	void save() const;
private:
	bool set_option(const std::string &, const std::string &);
	void load();
};

#endif
