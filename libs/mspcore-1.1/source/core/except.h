/* $Id: except.h 63 2008-12-24 07:01:36Z tdb $

This file is part of libmspcore
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_CORE_ERROR_H_
#define MSP_CORE_ERROR_H_

#include <exception>
#include <string>
#include "../debug/backtrace.h"

namespace Msp {

/**
Base class for all Msp exceptions.
*/
class Exception: public std::exception
{
private:
	std::string wot;
	std::string wer;
	Debug::Backtrace bt;

public:
	Exception(const std::string &);
	~Exception() throw() { }

	const char *what() const throw() { return wot.c_str(); }
	Exception &at(const std::string &) throw();
	const char *where() const throw() { return wer.c_str(); }
	const Debug::Backtrace &get_backtrace() const throw() { return bt; }
};

/**
Thrown when a function parameter has an invalid value.
*/
class InvalidParameterValue: public Exception
{
public:
	InvalidParameterValue(const std::string &w_): Exception(w_) { }
};

/**
Thrown when a lookup from a map fails.
*/
class KeyError: public Exception
{
private:
	std::string key;

public:
	KeyError(const std::string &w_): Exception(w_) { }
	KeyError(const std::string &w_, const std::string &k);
	~KeyError() throw() { }

	const std::string &get_key() const { return key; }
};

/**
Thrown when the current object state doesn't allow the requested action.
*/
class InvalidState: public Exception
{
public:
	InvalidState(const std::string &w_): Exception(w_) { }
};

/**
Thrown when the application is invoked with wrong parameters.
*/
class UsageError: public Exception
{
private:
	bool brief;

public:
	UsageError(const std::string &r, bool b=true): Exception(r), brief(b) { }
	bool get_brief() const { return brief; }
};

/**
Thrown when a system call fails.
*/
class SystemError: public Exception
{
private:
	int err;

public:
	SystemError(const std::string &, int);
	int get_error_code() const { return err; }

private:
	static std::string build_what(const std::string &, int);
};

template<typename E>
void throw_at(E e, const std::string &a)
{ e.at(a); throw e; }

} // namespace Msp

#endif
