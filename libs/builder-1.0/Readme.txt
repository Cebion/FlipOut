Builder
Copyright © 2006-2009 Mikkosoft Productions
Version 1.0 readme


1. Description

Builder is a program for building other programs, similar to make, scons and
others.  Its main design goals are minimizing duplication of configuration
between packages and being as fast as possible.  Features include integrated
configuration and dependency resolution for C and C++ sources.


2. Building Builder

Builder is normally built using itself.  However, if you just downloaded the
source and don't yet have a Builder binary, how is that possible?  To resolve
this problem, there's a script called bootstrap.sh in the Builder main
directory.  Before running it, make sure you have the following libraries
available:

MSP libraries: core datafile fs strings io

Others: sigc++-2.0

Since the MSP libraries are also normally built with Builder, the script will
need to have their sources available.  By default, it will look at the parent
directory of builder.  You can change this by setting the LIBPATH evironment
variable for the script.  If everything goes well, the script will build a
builder-stage1 binary and then proceed to build a normal version with it.


3. Using Builder to build projects

In the simplest and most common case, Builder is invoked in the package's root
directory without any arguments.  An arbitary working directory may be
specified with -C, or an alternative Build file with -f.

To build a subset of the targets known to the package, the desired targets can
be named on the command line.  Note that if -C is used, the targets will be
interpreted relative to that directory, not the one where Builder is invoked
from.

To change package configuration, arguments of the form key=value may be given.
By default they will only affect the current package, but with the -A switch,
all active packages may be configured at once.

By default, files will be installed in $HOME/local.  If you are not happy with
this, the --prefix switch may be used to use another directory.  Note that
files may be installed due to dependencies when building multiple packages at
once even if you don't specify the install target.


4. Using Builder in your project

Before going into details about Builder instruction files, it's beneficial to
understand the basic concepts used in Builder.  There are three kinds of major
entities: packages, components and targets.

Packages are distributable units of software.

Components make up the contents of a package.  They define the high-level
goals such as libraries and executables.

Targets are the individual files taking part in the build process.  They are
created automatically from components.

The canonical name for the insturction file is Build.  It is written in a
structured declarative language, with syntax somewhat resembling the C
language.  The basic unit is a statement, which consists of a keyword,
optional arguments and an optional block of sub-statements.  Each statement is
terminated with a semicolon.

Typically, the Build file for a project defines a single package:

  package "mylib"
  {
    ...
  };

Inside the package can be some informational statements:

  version "0.1";
  description "My awesome library";

Packages may also depend on other packages:

  require "otherlib";

pkg-config is used to locate dependencies, and failing that, some heuristics
are applied to find Builder-enabled packages.

To be useful, a package must define one or more components.  At present, there
are five types:
- program: Build an executable program
- library: Build a library (both shared and static)
- module: Build a module for a program
- tarball: Pack files up in a tarbal
- includes: Install headers for a library

Each of these statements takes the component name as an argument.  The target
name is composed of the component name and a possible platform-specific prefix
and suffix.  For example, the statement:

  library "mylib";

Will create a file called libmylib.so on Linux and libmylib.dll on Windows.
For headers component, the name defines a subdirectory of the prefix include
directory to install headers in.

Components must contain one or more source statements telling where sources
are found:

  source "mylib.cpp";

Directories may be specified; each file inside the directory will be added to
the source list.  The list is filtered according to the component type and only
applicable files are used.

If you want a component to be installed, you can specify so:

  install true;

The default is not to install any components.  The installation directory is
automatically determined from the component type.

For a complete example, see Builder's own Build file.


5. Contact information

The latest releases can be found at http://www.tdb.fi/builder.shtml

Bug reports, feature requests etc. may be sent to tdb@tdb.fi

Read-only SVN access is available at http://svn.tdb.fi/builder


6. License

Builder is copyright © 2006-2009  Mikko Rasa, Mikkosoft Productions

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Full license text can be found in License.txt.


7. Version history

0.9
- First released version
