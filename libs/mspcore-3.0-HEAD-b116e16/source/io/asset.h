#ifndef MSP_IO_ASSET_H_
#define MSP_IO_ASSET_H_

#include "seekable.h"

namespace Msp {
namespace IO {

/**
Opens a file from the application's assets.  On Android, this means the assets
contained within the APK.  On other platfoms, assets are located in the
directory indicated by FS::get_sys_data_dir().  Assets are always read-only.
*/
class Asset: public Seekable
{
private:
	struct Private;

	Private *priv;

public:
	Asset(const std::string &);
	~Asset();

	virtual void set_block(bool);
	virtual void set_inherit(bool);

private:
	virtual unsigned do_write(const char *, unsigned);
	virtual unsigned do_read(char *, unsigned);

public:
	virtual const Handle &get_handle(Mode);

	virtual SeekOffset seek(SeekOffset, SeekType);
	virtual SeekOffset tell() const;
};

} // namespace IO
} // namespace Msp

#endif
