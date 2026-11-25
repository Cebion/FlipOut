/* $Id$

This file is part of libmspstrings
Copyright © 2007 Mikko Rasa
Distributed under the LGPL
*/
#ifndef MSP_STRINGS_REGMATCH_H_
#define MSP_STRINGS_REGMATCH_H_

#include <string>
#include <vector>

namespace Msp {

/**
This class stores the result of a Regex being matched against a string.  If the
match was successful, the RegMatch object evaluates to true, allowing it to be
used in constructs like \code if(RegMatch match=regex.match("foo")) \endcode.

A RegMatch representing a successful match has one or more groups, indicating
matching parts of the string.  The first group (with index 0) indicates the
part matched by the whol regex.  Further groups, if present, indicate parts
matched by subregexes.  These are ordered from left to right, by the opening
parenthesis of the subregex.
*/
class RegMatch
{
public:
	/**
	A single subregex of the match.
	*/
	struct Group
	{
		bool match;       //< Whether or not this group matched
		unsigned begin;   //< First offset of the match
		unsigned end;     //< One-past-last offset
		unsigned length;  //< Length of the match (end-begin)
		std::string str;  //< The part of the string that matched

		Group(): match(false) { }
		operator bool() const { return match; }
	};
	typedef std::vector<Group> GroupArray;

	/**
	Constructs a RegMatch representig a non-match.  Used by Regex.
	*/
	RegMatch() { }

	/**
	Constructs a new RegMatch from a string and groups.  The length and str members
	of each group are computed and need not be set.  Used by Regex.
	*/
	RegMatch(const std::string &, const std::vector<Group> &);

	/**
	Returns a reference to a single group in the match.  An exception is thrown
	if the requested group does not exist.
	*/
	const Group &group(unsigned) const;

	/**
	Returns true if the RegMatch object represents a non-match.
	*/
	bool empty() const { return groups.empty(); }

	/**
	Returns the number of groups in this match.
	*/
	unsigned size() const { return groups.size(); }

	/**
	Returns the begin offset of the whole match.
	*/
	unsigned begin() const { return groups.empty()?0:groups[0].begin; }

	/**
	Returns the end offset of the whole match.
	*/
	unsigned end() const { return groups.empty()?0:groups[0].end; }

	/**
	Shortcut for the group() function.
	*/
	const Group &operator[](unsigned i) const { return group(i); }

	operator bool() const { return !empty(); }
private:
	std::vector<Group> groups;
};

} // namespace Msp

#endif
