/* $Id$

This file is part of libmspstrings
Copyright © 2007 Mikko Rasa
Distributed under the LGPL
*/

#include <stack>
#include <limits>
#include <msp/core/except.h>
#include "formatter.h"
#include "regex.h"

using namespace std;

namespace {

/**
Writes an integer to a Regex code string, in little-endian order.
*/
template<typename T>
void write_int(T n, Msp::Regex::Code &code)
{
	for(unsigned i=0; i<sizeof(T); ++i)
		code+=(n>>i*8)&0xFF;
}

/**
Reads an integer from a Regex code stream, in little-endian order.
*/
template<typename T>
T read_int(Msp::Regex::Code::const_iterator &c)
{
	T result=0;
	for(unsigned i=0; i<sizeof(T); ++i)
		result+=static_cast<unsigned char>(*c++)<<i*8;
	return result;
}

}

namespace Msp {

Regex::Regex(const string &expr)
{
	n_groups=0;
	string::const_iterator iter=expr.begin();
	code=compile(expr, iter, n_groups, false);
	++n_groups;
}

RegMatch Regex::match(const string &str) const
{
	RegMatch::GroupArray groups(n_groups);

	for(string::const_iterator i=str.begin(); i!=str.end(); ++i)
		if(run(str, i, groups))
			return RegMatch(str, groups);

	return RegMatch();
}

string Regex::disassemble() const
{
	ostringstream ss;

	for(Code::const_iterator i=code.begin(); i!=code.end();)
	{
		Code::const_iterator j=i;
		Offset offset=i-code.begin();
		string decompiled=disassemble_instruction(i);
		string bytes;
		for(; j!=i; ++j)
			bytes+=format(" %02X", static_cast<int>(*j)&0xFF);
		ss<<Fmt("%3d")<<offset<<':'<<Fmt("%-9s")<<bytes;
		if(bytes.size()>9)
			ss<<"\n"<<Fmt("%15s");
		ss<<"  "<<decompiled<<'\n';
	}

	return ss.str();
}

Regex::Code Regex::compile(const string &expr, string::const_iterator &iter, unsigned &group, bool branch)
{
	bool has_branches=false;
	unsigned level=0;
	bool escape=false;
	unsigned bracket=0;
	string::const_iterator end;
	for(end=iter; end!=expr.end(); ++end)
	{
		if(escape)
			escape=false;
		else if(bracket)
		{
			if(bracket==3 && *end==']')
				bracket=0;
			else if(bracket==1 && *end=='^')
				bracket=2;
			else
				bracket=3;
		}
		else if(*end=='\\')
			escape=true;
		else if(*end=='(')
			++level;
		else if(*end==')')
		{
			if(level==0)
			{
				if(group==0)
					throw InvalidParameterValue("Unexpected )");
				else
					break;
			}
			--level;
		}
		else if(*end=='|' && level==0)
		{
			if(branch)
				break;
			else
				has_branches=true;
		}
		else if(*end=='[')
			bracket=1;
	}

	if(level>0)
		throw InvalidParameterValue("Unmatched (");

	Code result;

	unsigned this_group=group;
	if(!branch)
	{
		result+=GROUP_BEGIN;
		write_int<Index>(this_group, result);
	}

	const unsigned jump_size=1+sizeof(Offset);

	if(!has_branches)
	{
		for(string::const_iterator i=iter; i!=end;)
		{
			Code atom=parse_atom(expr, i, group);

			Count repeat_min=1;
			Count repeat_max=1;
			parse_repeat(i, repeat_min, repeat_max);

			for(unsigned j=0; j<repeat_min; ++j)
				result+=atom;
			if(repeat_max==numeric_limits<Count>::max())
			{
				if(repeat_min==0)
				{
					result+=ND_JUMP;
					write_int<Offset>(atom.size()+jump_size, result);
					result+=atom;
				}
				result+=ND_JUMP;
				write_int<Offset>(-(atom.size()+jump_size), result);
			}
			else if(repeat_max>repeat_min)
			{
				for(unsigned j=repeat_min; j<repeat_max; ++j)
				{
					result+=ND_JUMP;
					write_int<Offset>((repeat_max-j)*(atom.size()+jump_size)-jump_size, result);
					result+=atom;
				}
			}
		}
	}
	else
	{
		list<Code> branches;
		for(string::const_iterator i=iter;;)
		{
			branches.push_back(compile(expr, i, group, true));
			if(i==end)
				break;
			++i;
		}

		unsigned n_branches=branches.size();

		Offset offset=(n_branches-1)*jump_size+branches.front().size();
		for(list<Code>::iterator i=++branches.begin(); i!=branches.end(); ++i)
		{
			result+=ND_JUMP;
			write_int<Offset>(offset, result);
			offset+=i->size();
		}

		for(list<Code>::iterator i=branches.begin(); i!=branches.end();)
		{
			result+=*i;
			offset-=i->size()+jump_size;
			++i;
			if(i!=branches.end())
			{
				result+=JUMP;
				write_int<Offset>(offset, result);
			}
		}
	}

	if(!branch)
	{
		result+=GROUP_END;
		write_int<Index>(this_group, result);
	}

	iter=end;

	return result;
}

Regex::Code Regex::parse_atom(const string &expr, string::const_iterator &i, unsigned &group)
{
	Code result;

	if(i==expr.end())
		return result;

	bool flag=false;
	if(*i=='\\')
	{
		if(++i==expr.end())
			throw InvalidParameterValue("Stray backslash");
		flag=true;
	}

	if(!flag)
	{
		if(*i=='*' || *i=='{' || *i=='}' || *i=='+' || *i=='?' || *i=='|' || *i==')')
			throw InvalidParameterValue("Invalid atom");
		else if(*i=='[')
			return parse_brackets(expr, i);
		else if(*i=='.')
			result+=MATCH_ANY;
		else if(*i=='^')
			result+=MATCH_BEGIN;
		else if(*i=='$')
			result+=MATCH_END;
		else if(*i=='(')
		{
			++group;
			result=compile(expr, ++i, group, false);
		}
		else
			flag=true;
	}

	if(flag)
	{
		result+=MATCH_CHAR;
		result+=*i;
	}

	++i;

	return result;
}

bool Regex::parse_repeat(string::const_iterator &i, Count &rmin, Count &rmax)
{
	if(*i!='*' && *i!='+' && *i!='?' && *i!='{')
		return false;

	if(*i=='*' || *i=='+')
		rmax=numeric_limits<Count>::max();
	if(*i=='*' || *i=='?')
		rmin=0;
	if(*i=='{')
	{
		rmin=0;
		for(++i; isdigit(*i); ++i)
			rmin=rmin*10+(*i-'0');

		if(*i==',')
		{
			++i;
			if(*i!='}')
			{
				rmax=0;
				for(; isdigit(*i); ++i)
					rmax=rmax*10+(*i-'0');
				if(rmax<rmin)
					throw InvalidParameterValue("Invalid bound");
			}
			else
				rmax=numeric_limits<Count>::max();
		}
		else
			rmax=rmin;
		if(*i!='}')
			throw InvalidParameterValue("Invalid bound");
	}

	++i;

	return true;
}

Regex::Code Regex::parse_brackets(const string &str, string::const_iterator &iter)
{
	Code result;

	++iter;
	bool neg=false;
	if(*iter=='^')
	{
		neg=true;
		++iter;
	}

	string::const_iterator end=iter;
	for(; (end!=str.end() && (end==iter || *end!=']')); ++end) ;
	if(end==str.end())
		throw InvalidParameterValue("Unmatched '['");

	unsigned char mask[32]={0};
	unsigned type=0;
	bool range=false;
	unsigned char first=0, last=0;
	for(string::const_iterator i=iter; i!=end; ++i)
	{
		unsigned char c=*i;
		if(range)
		{
			last=c;
			for(unsigned j=first; j<=c; ++j)
				mask[j>>3]|=1<<(j&7);
			range=false;
			if(type<2)
				type=2;
		}
		else if(c=='-' && i!=iter && end-i>1)
			range=true;
		else
		{
			first=c;
			mask[c>>3]|=1<<(c&7);
			if(type==0)
				type=1;
			else
				type=3;
		}
	}

	if(neg)
		result+=NEGATE;

	if(type==1)
	{
		result+=MATCH_CHAR;
		result+=first;
	}
	else if(type==2)
	{
		result+=MATCH_RANGE;
		result+=first;
		result+=last;
	}
	else
	{
		result+=MATCH_MASK;
		result.append(reinterpret_cast<char *>(mask), 32);
	}

	iter=end;
	++iter;

	return result;
}

bool Regex::run(const string &str, const string::const_iterator &begin, RegMatch::GroupArray &groups) const
{
	bool result=false;
	list<RunContext> ctx;
	ctx.push_back(RunContext());
	ctx.front().citer=code.begin();
	ctx.front().groups.resize(groups.size());

	for(string::const_iterator i=begin;;)
	{
		int c;
		if(i!=str.end())
			c=static_cast<unsigned char>(*i);
		else
			c=-1;

		for(list<RunContext>::iterator j=ctx.begin(); j!=ctx.end();)
		{
			bool terminate=false;
			bool negate_match=false;
			for(; j->citer!=code.end();)
			{
				Instruction instr=static_cast<Instruction>(*j->citer++);

				if(instr==NEGATE)
					negate_match=true;
				else if(instr==JUMP)
				{
					Offset offset=read_int<Offset>(j->citer);
					j->citer+=offset;
				}
				else if(instr==ND_JUMP)
				{
					Offset offset=read_int<Offset>(j->citer);
					ctx.push_back(*j);
					ctx.back().citer+=offset;
				}
				else if(instr==GROUP_BEGIN)
				{
					Index n=read_int<Index>(j->citer);
					if(!j->groups[n].match)
						j->groups[n].begin=i-str.begin();
				}
				else if(instr==GROUP_END)
				{
					Index n=read_int<Index>(j->citer);
					if(!j->groups[n].match)
					{
						j->groups[n].match=true;
						j->groups[n].end=i-str.begin();
						j->groups[n].length=j->groups[n].end-j->groups[n].begin;
					}

					if(n==0)
					{
						result=true;
						bool better=false;
						for(unsigned k=0; (k<groups.size() && !better); ++k)
						{
							better=group_compare(j->groups[k], groups[k]);
							if(group_compare(groups[k], j->groups[k]))
								break;
						}
						if(better)
							groups=j->groups;
					}
				}
				else
				{
					bool match_result=false;
					bool input_consumed=false;
					if(instr==MATCH_BEGIN)
						match_result=(i==str.begin());
					else if(instr==MATCH_END)
						match_result=(i==str.end());
					else if(instr==MATCH_CHAR)
					{
						match_result=(c==*j->citer++);
						input_consumed=true;
					}
					else if(instr==MATCH_RANGE)
					{
						unsigned char first=*j->citer++;
						unsigned char last=*j->citer++;
						match_result=(c>=first && c<=last);
						input_consumed=true;
					}
					else if(instr==MATCH_MASK)
					{
						if(c>=0 && c<=0xFF)
						{
							unsigned char m=*(j->citer+(c>>3));
							match_result=m&(1<<(c&7));
						}
						input_consumed=true;
						j->citer+=32;
					}
					else if(instr==MATCH_ANY)
					{
						match_result=true;
						input_consumed=true;
					}
					else
						throw Exception("Invalid instruction");

					if(match_result==negate_match)
						terminate=true;
					negate_match=false;

					if(input_consumed || terminate)
						break;
				}
			}

			if(terminate || j->citer==code.end())
				j=ctx.erase(j);
			else
				++j;
		}

		if(i==str.end() || ctx.empty())
			break;
		++i;
	}

	return result;
}

bool Regex::group_compare(const RegMatch::Group &g1, const RegMatch::Group &g2) const
{
	if(!g1.match)
		return false;

	// Any match is better than no match
	if(!g2.match)
		return true;

	// Earlier match is better
	if(g1.begin<g2.begin)
		return true;
	if(g2.begin>g2.begin)
		return false;

	// Longer match at same position is better
	return g1.end>g2.end;
}

string Regex::disassemble_instruction(Code::const_iterator &i) const
{
	Instruction instr=static_cast<Instruction>(*i++);

	ostringstream result;
	switch(instr)
	{
	case JUMP:
		{
			Offset offset=read_int<Offset>(i);
			result<<"JUMP "<<Fmt("%+d")<<offset<<" ("<<Fmt("%d")<<i-code.begin()+offset<<')';
		}
		break;
	case ND_JUMP:
		{
			Offset offset=read_int<Offset>(i);
			result<<"ND_JUMP "<<Fmt("%+d")<<offset<<" ("<<Fmt("%d")<<i-code.begin()+offset<<')';
		}
		break;
	case GROUP_BEGIN:
		result<<"GROUP_BEGIN "<<read_int<Index>(i);
		break;
	case GROUP_END:
		result<<"GROUP_END "<<read_int<Index>(i);
		break;
	case NEGATE:
		result<<"NEGATE";
		break;
	case MATCH_BEGIN:
		result<<"MATCH_BEGIN";
		break;
	case MATCH_END:
		result<<"MATCH_END";
		break;
	case MATCH_CHAR:
		{
			char c=*i++;
			result<<"MATCH_CHAR ";
			if(c>=0x20 && c<=0x7E)
				result<<'\''<<c<<'\'';
			else
				result<<(static_cast<int>(c)&0xFF);
		}
		break;
	case MATCH_RANGE:
		result<<"MATCH_RANGE "<<(static_cast<int>(*i++)&0xFF);
		result<<'-'<<(static_cast<int>(*i++)&0xFF);
		break;
	case MATCH_MASK:
		result<<"MATCH_MASK";
		for(unsigned j=0; j<32; ++j)
			result<<' '<<Fmt("%02X")<<(static_cast<int>(*i++)&0xFF);
		break;
	case MATCH_ANY:
		result<<"MATCH_ANY";
		break;
	default:
		result<<"UNKNOWN "<<instr;
	}

	return result.str();
}

} // namespace Msp
