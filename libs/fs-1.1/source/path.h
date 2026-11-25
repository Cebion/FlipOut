/* $Id$

This file is part of libmspfs
Copyright © 2006-2008  Mikko Rasa, Mikkosoft Productions
Distributed under the LGPL
*/

#ifndef MSP_FS_PATH_H_
#define MSP_FS_PATH_H_

#include <ostream>
#include <string>

namespace Msp {
namespace FS {

enum
{
#ifdef WIN32
	DIRSEP='\\'
#else
	DIRSEP='/'
#endif
};

/**
Stores a filesystem path.  Paths are always stored in a normalized form; there
are never any "." or ".." components in the middle of the path, and relative
paths always begin with a single "." component or a sequence ".." components.
*/
class Path
{
public:
	class Iterator
	{
		friend class Path;

	private:
		const Path &path;
		std::string::size_type start,end;

		Iterator(const Path &);
	public:
		Iterator    &operator++();
		Iterator    &operator--();
		std::string operator*() const;
		bool        operator==(const Iterator &i) const { return (start==i.start && end==i.end); }
		bool        operator!=(const Iterator &i) const { return !(*this==i); }
	};

private:
	std::string path;

public:
	Path();
	Path(const std::string &);
	Path(const char *);

	const std::string &str() const { return path; }

	/// Returns the number of components in the path.
	unsigned    size() const;

	bool        empty() const { return path.empty(); }

	/// Determines whether the path starts from the root directory
	bool is_absolute() const;

	/// Extracts a range of components from the path.
	Path subpath(unsigned start, unsigned count=static_cast<unsigned>(-1)) const;

	/// Concatenates this path with another one, with usual filesystem semantics
	Path operator/(const Path &p) const;
	Path &operator/=(const Path &);

	/**
	Extracts a single component from the path.  Negative indices count from the
	end of the path.
	*/
	std::string operator[](int) const;

	bool        operator==(const Path &) const;
	Iterator    begin() const;
	Iterator    end() const;
private:
	void init(const std::string &);
	void add_component(const std::string &);
};

inline std::ostream &operator<<(std::ostream &o, const Path &p) { o<<p.str(); return o; }

} // namespace FS
} // namespace Msp

#endif
