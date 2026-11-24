#ifndef MSP_STRINGS_REGMATCH_H_
#define MSP_STRINGS_REGMATCH_H_

#include <string>
#include <vector>

namespace Msp {

/**
This class stores the result of a Regex being matched against a string.  If the
match was successful, the RegMatch object evaluates to true.

A RegMatch representing a successful match has one or more groups, indicating
matching parts of the string.  The first group (with index 0) indicates the
part matched by the whole regex.  Further groups, if present, indicate parts
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
		typedef std::string::size_type size_type;

		bool match;        //< Whether or not this group matched
		size_type begin;   //< First offset of the match
		size_type end;     //< One-past-last offset
		size_type length;  //< Length of the match (end-begin)
		std::string str;   //< The part of the string that matched

		Group(): match(false) { }
		operator bool() const { return match; }
	};

private:
	std::vector<Group> groups;

public:
	/** Constructs a RegMatch representing a non-match. */
	RegMatch() { }

	/** Constructs a new RegMatch from a string and groups.  The length and str
	members of each group are computed and need not be set.  Intended to be used
	by the Regex class. */
	RegMatch(const std::string &, const std::vector<Group> &);

	/** Returns a reference to a single group in the match. */
	const Group &group(unsigned) const;

	/** Returns true if the RegMatch object represents a non-match. */
	bool empty() const { return groups.empty(); }

	/** Returns the number of groups in this match. */
	unsigned size() const { return groups.size(); }

	/** Returns the begin offset of the whole match. */
	unsigned begin() const { return groups.empty() ? 0 : groups[0].begin; }

	/** Returns the end offset of the whole match. */
	unsigned end() const { return groups.empty() ? 0 : groups[0].end; }

	/** Shorthand for the group() function. */
	const Group &operator[](unsigned i) const { return group(i); }

	operator bool() const { return !empty(); }
};

} // namespace Msp

#endif
