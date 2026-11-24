/* $Id: textparser.cpp 45 2008-08-06 16:35:21Z tdb $

This file is part of libmspdatafile
Copyright © 2007-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <msp/strings/formatter.h>
#include <msp/strings/utils.h>
#include "input.h"
#include "textparser.h"
#include "token.h"

using namespace std;

namespace Msp {
namespace DataFile {

TextParser::TextParser(Input &i, const string &s):
	ParserMode(i, s)
{ }

Statement TextParser::parse()
{
	return parse_statement(0);
}

Statement TextParser::parse_statement(const Token *t)
{
	Statement result;
	bool      sub=false;
	bool      finish=false;

	while(in)
	{
		Token token;
		if(t)
		{
			token=*t;
			t=0;
		}
		else
			token=parse_token();

		if(result.keyword.empty())
		{
			if(token.str.empty())
				break;
			else if(token.type!=Token::IDENTIFIER)
				throw_at(ParseError(format("Syntax error at token '%s' (expected an identifier)", token.str)), get_location());
			result.keyword=token.str;
			result.valid=true;
			result.source=src;
			result.line=in.get_line_number();
		}
		else if(sub)
		{
			if(token.str=="}")
			{
				sub=false;
				finish=true;
			}
			else
			{
				Statement ss=parse_statement(&token);
				result.sub.push_back(ss);
			}
		}
		else if(finish)
		{
			if(token.str!=";")
				throw_at(ParseError(format("Syntax error at token '%s' (Expected a ';')", token.str)), get_location());
			break;
		}
		else if(token.str=="{")
			sub=true;
		else if(token.str==";")
			break;
		else if(token.type==Token::INTEGER)
			result.args.push_back(Value(INTEGER, token.str));
		else if(token.type==Token::FLOAT)
			result.args.push_back(Value(FLOAT, token.str));
		else if(token.type==Token::STRING)
			result.args.push_back(Value(STRING, token.str));
		else if(token.type==Token::IDENTIFIER)
		{
			if(token.str=="true")
				result.args.push_back(Value(BOOLEAN, "1"));
			else if(token.str=="false")
				result.args.push_back(Value(BOOLEAN, "0"));
			else
				result.args.push_back(Value(ENUM, token.str));
			//result.args.push_back(resolve_identifiertoken.str);
		}
		else if(token.str=="")
			throw_at(ParseError("Unexcepted end of input"), get_location());
		else
			throw_at(ParseError("Syntax error"), get_location());
	}

	return result;
}

Token TextParser::parse_token()
{
	int c=0;
	int comment=0;

	// Skip over comments and whitespace
	while(in && comment>=0)
	{
		c=in.get();
		int next=in.peek();

		if(c=='/' && next=='/')
			comment=1;
		else if(c=='/' && next=='*')
			comment=2;
		else if(c=='\n' && comment==1)
			comment=0;
		else if(c=='*' && next=='/' && comment==2)
			comment=3;
		else if(comment==3)   // Skip the second character of block comment end
			comment=0;
		else if(!isspace(c) && !comment)
			comment=-1;
	}

	if(comment>0)  // EOF while in comment
		throw_at(ParseError("Unfinished comment at end of input"), get_location());
	else if(comment==0)  // Didn't hit any non-whitespace
		return Token(Token::SPECIAL, "");

	enum ParseState
	{
		INIT,
		SIGN,
		FLOATEXPINIT,
		FLOATEXPSIGN,
		STRING,
		ACCEPT,
		ZERO,
		DECIMAL,
		HEXADECIMAL,
		OCTAL,
		FLOAT,
		FLOATEXP,
		IDENTIFIER
	};

	static Token::Type token_type[]=
	{
		Token::SPECIAL,
		Token::SPECIAL,
		Token::SPECIAL,
		Token::SPECIAL,
		Token::STRING,
		Token::SPECIAL,
		Token::INTEGER,
		Token::INTEGER,
		Token::INTEGER,
		Token::INTEGER,
		Token::FLOAT,
		Token::FLOAT,
		Token::IDENTIFIER
	};

	ParseState state=INIT;
	string     buf;
	bool       escape=false;

	while(in || state==INIT)
	{
		if(state!=INIT)
			c=in.get();
		int next=in.peek();

		buf+=c;

		switch(state)
		{
		case INIT:
			if(c=='0')
				state=ZERO;
			else if(c=='-' || c=='+')
				state=SIGN;
			else if(c=='.')
				state=FLOAT;
			else if(c=='"')
				state=STRING;
			else if(c=='{' || c=='}' || c==';')
				return Token(Token::SPECIAL, string(1, c));
			else if(isdigit(c))
				state=DECIMAL;
			else if(isalpha(c) || c=='_')
				state=IDENTIFIER;
			else
				parse_error(c, "0-9A-Za-z_.\"{};+-");
			break;

		case SIGN:
			if(c=='0')
				state=ZERO;
			else if(isdigit(c))
				state=DECIMAL;
			else if(c=='.')
				state=FLOAT;
			else
				parse_error(c, "0-9.");
			break;

		case ZERO:
			if(c=='x')
				state=HEXADECIMAL;
			else if(isdigit(c))
				state=OCTAL;
			else if(c=='.')
				state=FLOAT;
			else
				parse_error(c, "0-9A-Fa-f.");
			break;

		case DECIMAL:
			if(c=='.')
				state=FLOAT;
			else if(!isdigit(c))
				parse_error(c, "0-9.");
			break;

		case HEXADECIMAL:
			if(!isxdigit(c))
				parse_error(c, "0-9A-Fa-f");
			break;

		case OCTAL:
			if(!isodigit(c))
				parse_error(c, "0-7");
			break;

		case FLOAT:
			if(c=='e' || c=='E')
				state=FLOATEXPINIT;
			else if(!isdigit(c))
				parse_error(c, "0-9Ee");
			break;

		case FLOATEXPINIT:
			if(c=='+' || c=='-')
				state=FLOATEXPSIGN;
			else if(isdigit(c))
				state=FLOATEXP;
			else
				parse_error(c, "0-9+-");
			break;

		case FLOATEXPSIGN:
			if(isdigit(c))
				state=FLOATEXP;
			else
				parse_error(c, "0-9");
			break;

		case FLOATEXP:
			if(!isdigit(c))
				parse_error(c, "0-9");
			break;

		case STRING:
			if(c=='\\')
				escape=!escape;
			else if(c=='"' && !escape)
			{
				try
				{
					return Token(Token::STRING, c_unescape(buf.substr(1, buf.size()-2)));
				}
				catch(Exception &e)
				{
					e.at(get_location());
					throw;
				}
			}
			else
				escape=false;
			break;

		case IDENTIFIER:
			if(!isalpha(c) && !isdigit(c) && c!='_')
				parse_error(c, "0-9A-Za-z_");
			break;

		default:
			throw_at(InvalidState("Internal error (bad state)"), get_location());
		}

		if(is_delimiter(next) && state>=ACCEPT)
			return Token(token_type[state], buf);
	}

	return Token(Token::SPECIAL, "");
}

bool TextParser::is_delimiter(int c)
{
	return (isspace(c) || c=='{' || c=='}' || c==';' || c=='/');
}

bool TextParser::isodigit(int c)
{
	return (c>='0' && c<='7');
}

string TextParser::get_location()
{
	ostringstream ss;
	ss<<src<<':'<<in.get_line_number();
	return ss.str();
}

void TextParser::parse_error(int c, const char *e)
{
	throw_at(ParseError(format("Parse error at '%c', expected one of \"%s\"", static_cast<char>(c), e)), get_location());
}

} // namespace DataFile
} // namespace Msp
