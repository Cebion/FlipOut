#ifndef MSP_FS_PATH_H_
#define MSP_FS_PATH_H_

#include <ostream>
#include <string>
#include <vector>

namespace Msp {
namespace FS {

enum
{
#ifdef _WIN32
	DIRSEP = '\\'
#else
	DIRSEP = '/'
#endif
};

/**
Stores a filesystem path.  Paths are always stored in a normalized form; there
are never any "." or ".." components in the middle of the path, and relative
paths always begin with a single "." component or a sequence ".." components.
As a special case, calling the constructor with no arguments or with an empty
string will construct an empty path; this can be usedful as an invalid value.

A path can also be treated as an array of components, supporting indexing,
iteration and slicing.  In this context the root directory is treated as a
component of its own.
*/
class Path
{
private:
	typedef std::vector<std::string::size_type> PositionArray;

public:
	class Iterator
	{
	public:
		typedef PositionArray::difference_type difference_type;
		typedef const std::string value_type;
		typedef const std::string *pointer;
		typedef const std::string &reference;
		typedef std::input_iterator_tag iterator_category;

	private:
		const Path *path;
		PositionArray::const_iterator iter;
		bool end;
		std::string current;

		Iterator(const Path &, bool = false);
	public:
		static Iterator at_begin(const Path &p) { return Iterator(p); }
		static Iterator at_end(const Path &p) { return Iterator(p, true); }

		Iterator &operator++();
		Iterator operator++(int) { Iterator i = *this; ++*this; return i; }
		Iterator &operator--();
		const std::string &operator*() const { return current; }
		const std::string *operator->() const { return &current; }
		bool operator==(const Iterator &i) const { return (iter==i.iter && end==i.end); }
		bool operator!=(const Iterator &i) const { return !(*this==i); }
	private:
		void update();
	};

private:
	std::string path;
	PositionArray separators;

public:
	Path();
	Path(const std::string &);
	Path(const char *);
private:
	void init(const std::string &);

public:
	/// Returns the path as a string.
	const std::string &str() const { return path; }

	/// Returns the path as a pointer to a null-terminated string.
	const char *c_str() const { return path.c_str(); }

	/// Returns the number of components in the path.
	unsigned size() const;

	/** Indicates whether the path is empty.  This really means empty; a path
	pointing to the current directory is not empty. */
	bool empty() const { return path.empty(); }

	/// Determines whether the path starts from the root directory.
	bool is_absolute() const;

	/// Extracts a range of components from the path.
	Path subpath(unsigned start, unsigned count = static_cast<unsigned>(-1)) const;

	/// Concatenates this path with another one, with usual filesystem semantics.
	Path operator/(const Path &p) const;
	Path &operator/=(const Path &);

private:
	/** Adds a component to the path.  It must not contain the directory
	separator character. */
	void add_component(const std::string &);

public:
	/** Extracts a single component from the path.  Negative indices count from
	the end of the path. */
	std::string operator[](int) const;

	bool operator==(const Path &) const;
	bool operator<(const Path &) const;
	bool operator>(const Path &) const;
	bool operator<=(const Path &other) const { return !(*this>other); }
	bool operator>=(const Path &other) const { return !(*this<other); }
	bool operator!=(const Path &other) const { return !(*this==other); }

	Iterator begin() const { return Iterator::at_begin(*this); }
	Iterator end() const { return Iterator::at_end(*this); }
};

inline std::ostream &operator<<(std::ostream &o, const Path &p) { o<<p.str(); return o; }

} // namespace FS
} // namespace Msp

#endif
