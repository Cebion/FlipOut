#include <msp/fs/dir.h>
#include "asset.h"
#include "file.h"

using namespace std;

namespace Msp {
namespace IO {

struct Asset::Private
{
	Seekable *file;
};

Asset::Asset(const string &name)
{
	Seekable *file = new BufferedFile((FS::get_sys_data_dir()/name).str());

	priv = new Private;
	priv->file = file;

	priv->file->signal_flush_required.connect(signal_flush_required);
}

Asset::~Asset()
{
	delete priv->file;
	delete priv;
}

unsigned Asset::do_read(char *buf, unsigned size)
{
	return priv->file->read(buf, size);
}

SeekOffset Asset::seek(SeekOffset off, SeekType type)
{
	return priv->file->seek(off, type);
}

SeekOffset Asset::tell() const
{
	return priv->file->tell();
}

} // namespace IO
} // namespace Msp
