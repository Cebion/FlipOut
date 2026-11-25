/* $Id$

This file is part of libmspstrings
Copyright © 2007 Mikko Rasa
Distributed under the LGPL
*/
#ifndef MSP_STRINGS_REGEX_H_
#define MSP_STRINGS_REGEX_H_

#include <string>
#include "regmatch.h"

namespace Msp {

/**
This class provides regular expression matching.  It supports a subset of
POSIX.2 extended regex syntax.  Character classes, equivalence classes and
collating elements are not supported.  Refer to the regex(7) manpage for more
details.

A description of the internal workings of the class follows.  You may skip this
if you are not a developer or otherwise interested.

The core of this class is a virtual machine running a non-deterministic finite
automaton (NFA).  The state of the automaton is represented by an iterator into
the code.  Transitions between states are represented by instructions.
Instructions may take multiple bytes, so a valid code iterator may not be a
valid NFA state.

The virtual machine may have any number of execution contexts at any one time.
On every cycle, each context is advanced until an input-consuming instruction
is encountered, keeping the input positions in sync with each other.  Execution
continues as long as there's at least one context remaining.

The GROUP_BEGIN and GROUP_END instructions record the begin and end offset of a
match group, respectively.  The GROUP_END instruction also marks the group as
successfully matched.  If the target group was already matched, these
instructions do nothing.

The JUMP instruction causes the execution iterator to be modified by an offset.

The ND_JUMP instruction causes the execution context to be split in two.  One
continues directly after the instruction and the other continues at an offset.

The NEGATE instruction causes the result of the next match instruction to be
negated.

Match instructions compare the input against a condition.  If the match
succeeds, execution continues at the next instruction.  If the match fails,
execution of that context is terminated.

The MATCH_BEGIN and MATCH_END instructions match the beginning and end of
the input string, respectively.  They do not consume input.

The MATCH_CHAR instruction consumes the input character and matches it against
a single character.  Since regexes often match sequences of printable character,
a match for a non-opcode character may be encoded as the character itself.

The MATCH_RANGE instruction consumes the input character and matches it against
an inclusive character range.

The MATCH_MASK instruction consumes the input character and matches it against
a bitmask.

The MATCH_ANY instruction consumes the input character and always succeeds.
*/
class Regex
{
public:
	/**
	Constructs a new Regex object.
	*/
	Regex(const std::string &expr);

	/**
	Matches the regex against a string.  Refer to RegMatch documentation for
	more information on the resulting object.
	*/
	RegMatch match(const std::string &str) const;

	/**
	Returns a disassembled representation of the NFA bytecode.  For debugging
	purposes.
	*/
	std::string disassemble() const;
private:
	typedef std::string Code;
	typedef unsigned short Count;
	typedef short Offset;
	typedef unsigned short Index;

	enum Instruction
	{
		FIRST_INSTRUCTION_=0,

		JUMP,
		ND_JUMP,

		GROUP_BEGIN,
		GROUP_END,

		NEGATE,

		MATCH_BEGIN,
		MATCH_END,
		MATCH_CHAR,
		MATCH_RANGE,
		MATCH_MASK,
		MATCH_ANY,

		LAST_INSTRUCTION_=31
	};

	struct RunContext
	{
		Code::const_iterator citer;
		RegMatch::GroupArray groups;
	};

	Code code;
	unsigned n_groups;

	/**
	Compiles a regular expression into NFA bytecode.  When compiling a whole
	regex, \a group should be set to 0.  When the function returns, \a group will
	be the index of the last subexpression and \a iter will point to the first
	unused character in the expression.

	\param   expr    Expression to be compiled
	\param   begin   Iterator into the expression
	\param   group   Group counter, gets incremented for each subregex
	\param   branch  Whether we are compiling a branch

	\return  Compiled NFA bytecode
	*/
	Code compile(const std::string &expr, std::string::const_iterator &iter, unsigned &group, bool branch);
	Code parse_atom(const std::string &, std::string::const_iterator &i, unsigned &);
	Code parse_brackets(const std::string &, std::string::const_iterator &);
	bool parse_repeat(std::string::const_iterator &, Count &, Count &);
	bool run(const std::string &, const std::string::const_iterator &, RegMatch::GroupArray &) const;
	bool group_compare(const RegMatch::Group &, const RegMatch::Group &) const;
	std::string disassemble_instruction(Code::const_iterator &) const;
};

} // namespace Msp

#endif
