/* $Id: vertexformat.cpp 73 2009-01-14 19:19:18Z tdb $

This file is part of libmspgl
Copyright © 2007-2009  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#include <cstring>
#include <msp/strings/lexicalcast.h>
#include "except.h"
#include "vertexformat.h"

namespace Msp {
namespace GL {

VertexFormat::VertexFormat():
	data(0)
{ }

VertexFormat::VertexFormat(VertexComponent c):
	data(new unsigned char[8])
{
	data[0]=1;
	data[1]=c;
}

VertexFormat::VertexFormat(const VertexFormat &f):
	data(0)
{
	if(f.data)
	{
		data=new unsigned char[f.data[0]/8+8];
		memcpy(data, f.data, f.data[0]+1);
	}
}

VertexFormat &VertexFormat::operator=(const VertexFormat &f)
{
	delete[] data;
	if(f.data)
	{
		data=new unsigned char[f.data[0]/8+8];
		memcpy(data, f.data, f.data[0]+1);
	}
	else
		data=0;

	return *this;
}

VertexFormat::~VertexFormat()
{
	delete[] data;
}

VertexFormat operator,(const VertexFormat &f, VertexComponent c)
{
	VertexFormat r=f;
	if(r.data)
	{
		const unsigned char n=++r.data[0];
		if((n&7)==7)
		{
			unsigned char *newdt=new unsigned char[n+9];
			memcpy(newdt, r.data, n);
			delete r.data;
			r.data=newdt;
		}
		r.data[n]=c;
	}
	else
	{
		r.data=new unsigned char[8];
		r.data[0]=1;
		r.data[1]=c;
	}

	return r;
}

VertexFormat operator,(const VertexFormat &f, unsigned i)
{
	if(!f.data)
		throw InvalidState("VertexFormat has no components");
	VertexFormat r=f;
	unsigned char *c=r.data+r.data[0];
	if(*c<ATTRIB1)
		throw InvalidState("Last component is not a generic attribute");
	// VertexArray uses an unsigned to store flags for enabled arrays
	if(i>=28)
		throw InvalidParameterValue("Generic attribute index out of range");
	*c+=i*4;

	return r;
}

uint get_stride(const VertexFormat &f)
{
	uint stride=0;
	for(const unsigned char *i=f.begin(); i!=f.end(); ++i)
		stride+=(*i&3)+1;
	return stride;
}

std::istream &operator>>(std::istream &in, VertexFormat &f)
{
	std::string str;
	in>>str;

	unsigned start=0;

	while(1)
	{
		unsigned underscore=str.find('_', start);
		bool fail=false;
		if(!str.compare(start, underscore-start, "VERTEX2"))
			f=(f,VERTEX2);
		else if(!str.compare(start, underscore-start, "VERTEX3"))
			f=(f,VERTEX3);
		else if(!str.compare(start, underscore-start, "VERTEX4"))
			f=(f,VERTEX4);
		else if(!str.compare(start, underscore-start, "NORMAL3"))
			f=(f,NORMAL3);
		else if(!str.compare(start, underscore-start, "TEXCOORD1"))
			f=(f,TEXCOORD1);
		else if(!str.compare(start, underscore-start, "TEXCOORD2"))
			f=(f,TEXCOORD2);
		else if(!str.compare(start, underscore-start, "TEXCOORD3"))
			f=(f,TEXCOORD3);
		else if(!str.compare(start, underscore-start, "TEXCOORD4"))
			f=(f,TEXCOORD4);
		else if(!str.compare(start, underscore-start, "COLOR4UB"))
			f=(f,COLOR4_UBYTE);
		else if(!str.compare(start, underscore-start, "COLOR3F"))
			f=(f,COLOR3_FLOAT);
		else if(!str.compare(start, underscore-start, "COLOR4F"))
			f=(f,COLOR4_FLOAT);
		else if(underscore>=start+8 && !str.compare(start, 6, "ATTRIB"))
		{
			try
			{
				char n=str[start+6];
				unsigned i=lexical_cast<unsigned>(str.substr(start+7, underscore-start-7));
				if(n=='1')
					f=(f,ATTRIB1,i);
				else if(n=='2')
					f=(f,ATTRIB2,i);
				else if(n=='3')
					f=(f,ATTRIB3,i);
				else if(n=='4')
					f=(f,ATTRIB4,i);
				else
					fail=true;
			}
			catch(const LexicalError &)
			{
				fail=true;
			}
		}
		else
			fail=true;

		if(fail)
		{
			in.setstate(std::ios_base::failbit);
			break;
		}

		if(underscore==std::string::npos)
			break;
		start=underscore+1;
	}

	return in;
}

} // namespace GL
} // namespace Msp
