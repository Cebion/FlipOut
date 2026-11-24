#ifndef MSP_IO_HANDLE_H_
#define MSP_IO_HANDLE_H_

#include <cstddef>

namespace Msp {
namespace IO {

class Handle
{
public:
	struct Private;

private:
	Private *priv;

public:
	Handle();
	Handle(const Handle &);
	Handle &operator=(const Handle &);
	~Handle();

	Private &operator*() { return *priv; }
	const Private &operator*() const { return *priv; }

	/** This is effectively a boolean conversion, but avoids accidental
	conversion to OS native handles.  Unix-based systems use int and win32 uses
	void *; const void * is not implicitly convertible to either. */
	operator const void *() const;
};


void sys_set_blocking(Handle &, bool);
void sys_set_inherit(Handle &, bool);
std::size_t sys_read(Handle &, char *, std::size_t);
std::size_t sys_write(Handle &, const char *, std::size_t);
void sys_close(Handle &);

} // namespace IO
} // namespace Msp

#endif
