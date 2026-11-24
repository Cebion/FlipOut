#include <stdexcept>
#include <msp/core/application.h>
#include <msp/core/mainthread.h>
#include <android/asset_manager.h>
#include "asset.h"
#include "file.h"

using namespace std;

namespace Msp {
namespace IO {

struct Asset::Private
{
	AAsset *asset;
};

Asset::Asset(const string &name)
{
	AAssetManager *asset_manager = reinterpret_cast<Android::MainThread *>(Application::get_data())->get_asset_manager();
	AAsset *asset = AAssetManager_open(asset_manager, name.c_str(), AASSET_MODE_RANDOM);
	if(!asset)
		throw file_not_found(name);

	priv = new Private;
	priv->asset = asset;

	mode = M_READ;
}

Asset::~Asset()
{
	AAsset_close(priv->asset);
	delete priv;
}

unsigned Asset::do_read(char *buf, unsigned size)
{
	int ret = AAsset_read(priv->asset, buf, size);
	if(ret<0)
		throw runtime_error("Asset::do_read");
	else if(ret==0)
		set_eof();

	return ret;
}

SeekOffset Asset::seek(SeekOffset off, SeekType type)
{
	signal_flush_required.emit();
	off = AAsset_seek(priv->asset, off, type);
	if(off<0)
		throw runtime_error("Asset::seek");
	return off;
}

SeekOffset Asset::tell() const
{
	SeekOffset off = AAsset_seek(priv->asset, 0, S_CUR);
	if(off<0)
		throw runtime_error("Asset::tell");
	return off;
}

} // namespace IO
} // namespace Msp
