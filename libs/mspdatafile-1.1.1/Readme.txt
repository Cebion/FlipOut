libmspdatafile - structured datafile library
Version 1.0 readme


1. Description

Libmspdatafile is a library for reading and writing structured data files.  It
supports both a human-readable text format and a more compact binary format.
The text-based format is designed to be compact yet clean, avoiding much of the
redundancy of XML.


2. Technical overview

The basic building block of a datafile is a statement.  A statement has a
keyword and zero or more arguments.  It may also have any number of substate-
ments.

The interface is heavily object-oriented and typically one statement in a file
represents one object in the program's memory.

The library supports five basic data types: integers, floats, strings, booleans
and enums.  Data types are strictly enforced - the only allowed conversion is
from integer to float.

The correct argument types for a statement is determined from the function or
variable used to load it.  This means that semantical validation can only be
done by the program that actually uses the file.  A generic tool can only
perform syntactic validation.


3. Basic usage

Loading data from files is achieved through Loader classes.  Such a class must
be derived from the DataFile::Loader class.  In the constructor, a series of
calls to the various overloads of the add() function is executed, telling the
loader what to do.  The two basic actions are calling a function of the loader
with the statement's arguments, or assigning the arguments to variables of the
loaded object.

There is a load() function in namespace scope that allows loading a whole file
into an object.

It is also possible to call the Parser::parse function directly and obtain a
raw Statement.  However, this is rarely useful.


4. Datafile syntax

The syntax of the text format vaguely resembles C.  Statements are terminated
with a semicolon and substatements are enclosed between burly braces.  The sub-
statement block comes after all arguments and the semicolon comes after it.  An
empty substatement block is allowed and equal to an absent one.

Integers can be written in decimal, octal or hexadecimal notation.  A minus
sign can be used in any base for negative numbers, as well as an optional plus
sign for positive numbers.

Floats must be written in decimal and they must contain a decimal point.  An
exponent may come after the mantissa, separated by the character e.  Minus and
plus signs are allowed in both exponent and mantissa.

Strings are enclosed in double quotes.  Backslash can be used to escape a
literal double quote or backslash.

Booleans are either true or false.

Enumeration values are represented as symbolic identifiers.  They may contain
alphaumeric characters and underscores and must begin with a letter or an
underscore.


5. Binary format

The binary format is not intended to be manipulated by anything else than
libmspdatafile.  A detailed description is not available at this time.

The tool provided with the library can be used to convert between the two
formats.


6. License

Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Full license text can be found in License.txt.


($Id: Readme.txt 46 2008-08-11 17:05:51Z tdb $)
