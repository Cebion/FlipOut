/* $Id: value.h 56 2008-12-15 11:49:33Z tdb $

This file is part of libmspdatafile
Copyright © 2006  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/
#ifndef MSP_DATAFILE_VALUE_H_
#define MSP_DATAFILE_VALUE_H_

#include <vector>
#include <msp/strings/lexicalcast.h>
#include "except.h"

namespace Msp {
namespace DataFile {

enum Type
{
	INTEGER,
	FLOAT,
	STRING,
	BOOLEAN,
	ENUM
};

template<typename T> struct TypeResolver       { static const Type type=ENUM; };
template<> struct TypeResolver<short>          { static const Type type=INTEGER; };
template<> struct TypeResolver<unsigned short> { static const Type type=INTEGER; };
template<> struct TypeResolver<int>            { static const Type type=INTEGER; };
template<> struct TypeResolver<unsigned>       { static const Type type=INTEGER; };
template<> struct TypeResolver<long>           { static const Type type=INTEGER; };
template<> struct TypeResolver<unsigned long>  { static const Type type=INTEGER; };
template<> struct TypeResolver<long long>      { static const Type type=INTEGER; };
template<> struct TypeResolver<unsigned long long> { static const Type type=INTEGER; };
template<> struct TypeResolver<float>          { static const Type type=FLOAT; };
template<> struct TypeResolver<double>         { static const Type type=FLOAT; };
template<> struct TypeResolver<bool>           { static const Type type=BOOLEAN; };
template<> struct TypeResolver<std::string>    { static const Type type=STRING; };
template<typename T> struct TypeResolver<const T>   { static const Type type=TypeResolver<T>::type; };
template<typename T> struct TypeResolver<T &>       { static const Type type=TypeResolver<T>::type; };
template<typename T> struct TypeResolver<const T &> { static const Type type=TypeResolver<T>::type; };

class Value
{
public:
	Value(Type t, const std::string &d): type(t), data(d) { }

	template<typename T>
	Value(T d): type(TypeResolver<T>::type), data(lexical_cast(d)) { }

	template<typename T>
	T get() const;

	Type get_type() const { return type; }
	const std::string &get_raw() const { return data; }
private:
	Type type;
	std::string data;
};

typedef std::vector<Value> ValueArray;

template<Type T> inline bool check_type(Type t)  { return t==T; }
template<> inline bool check_type<FLOAT>(Type t) { return t==INTEGER || t==FLOAT; }

template<typename T>
inline T Value::get() const
{
	if(!check_type<TypeResolver<T>::type>(type))
		throw TypeError("Type mismatch");

	return lexical_cast<T>(data);
}

template<>
inline std::string Value::get<std::string>() const
{
	if(type!=STRING)
		throw TypeError("Type mismatch");
	return data;
}

template<>
inline const std::string &Value::get<const std::string&>() const
{
	if(type!=STRING)
		throw TypeError("Type mismatch");
	return data;
}

} // namespace DataFile
} // namespace Msp

#endif
