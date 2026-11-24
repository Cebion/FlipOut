/* $Id: getopt.h 67 2009-09-11 17:34:36Z tdb $

This file is part of libmspcore
Copyright © 2006-2007 Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_CORE_GETOPT_H_
#define MSP_CORE_GETOPT_H_

#include <sstream>
#include <string>
#include <vector>
#include "except.h"

namespace Msp {

class GetOpt
{
public:
	enum ArgType
	{
		NO_ARG,
		OPTIONAL_ARG,
		REQUIRED_ARG
	};
	
	class OptBase
	{
	public:
		OptBase           &set_help(const std::string &);
		OptBase           &set_help(const std::string &, const std::string &);
		char              get_short() const               { return shrt; }
		const std::string &get_long() const               { return lng; }
		ArgType           get_arg_type() const            { return arg_type; }
		const std::string &get_help() const               { return help; }
		const std::string &get_metavar() const            { return metavar; }
		unsigned          get_seen_count() const          { return seen_count; }
		void              process();
		void              process(const std::string &);
		virtual ~OptBase() { }
	protected:
		char        shrt;
		std::string lng;
		ArgType     arg_type;
		unsigned    seen_count;
		std::string help;
		std::string metavar;

		OptBase(char, const std::string &, ArgType);
		virtual void store()=0;
		virtual void store(const std::string &)=0;
	};

private:
	template<typename T>
	class Option: public OptBase
	{
	public:
		Option(char s, const std::string &l, T &d, ArgType a): OptBase(s, l, a), data(d) { }

		virtual void store() { }

		virtual void store(const std::string &a)
		{
			T tmp;
			std::istringstream ss(a);
			ss>>tmp;
			if(ss.fail())
				throw UsageError("Invalid argument for --"+lng);

			data=tmp;
		}
	private:
		T &data;
	};

	template<typename T>
	class ListOption: public OptBase
	{
	public:
		ListOption(char s, const std::string &l, T &d, ArgType a): OptBase(s, l, a), data(d)
		{ if(arg_type!=REQUIRED_ARG) throw Exception("ListOption with arg_type!=REQUIRED makes no sense"); }

		virtual void store() { }

		virtual void store(const std::string &a)
		{
			typename T::value_type tmp;
			std::istringstream ss(a);
			ss>>tmp;
			if(ss.fail())
				throw UsageError("Invalid argument for --"+lng);

			data.push_back(tmp);
		}
	private:
		T &data;
	};

	std::list<OptBase *> opts;
	std::vector<std::string> args;

public:
	~GetOpt();

	const std::vector<std::string> &get_args() const { return args; }

	template<typename T>
	OptBase &add_option(char s, const std::string &l, T &d, ArgType a=NO_ARG)
	{ opts.push_back(new Option<T>(s, l, d, a)); return *opts.back(); }
	
	template<typename T>
	OptBase &add_option(char s, const std::string &l, std::list<T> &d, ArgType a=REQUIRED_ARG)
	{ opts.push_back(new ListOption<std::list<T> >(s, l, d, a)); return *opts.back(); }
	
	template<typename T>
	OptBase &add_option(const std::string &l, T &d, ArgType a)
	{ return add_option(0, l, d, a); }

	std::string generate_usage(const std::string &) const;
	std::string generate_help() const;
	void operator()(unsigned, const char *const *);
private:

	OptBase &get_option(char);
	OptBase &get_option(const std::string &);
	unsigned process_long(const char *const *);
	unsigned process_short(const char *const *);
};

template<> inline void GetOpt::Option<bool>::store()     { data=true; }
template<> inline void GetOpt::Option<unsigned>::store() { ++data; }

template<> inline void GetOpt::Option<std::string>::store(const std::string &a)
{ data=a; }

template<> inline void GetOpt::ListOption<std::list<std::string> >::store(const std::string &a)
{ data.push_back(a); }

} // namespace Msp

#endif
