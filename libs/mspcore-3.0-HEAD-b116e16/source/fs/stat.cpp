#include "path.h"
#include "stat.h"
#include "stat_private.h"

using namespace std;

namespace Msp {
namespace FS {

Stat::Private::Private():
	owner_id(0),
	group_id(0)
{ }


Stat::Stat():
	exists(false),
	type(UNKNOWN),
	size(0),
	alloc_size(0),
	priv(0)
{ }

Stat::Stat(const Stat &other):
	exists(other.exists),
	type(other.type),
	size(other.type),
	alloc_size(other.alloc_size),
	mtime(other.mtime),
	owner_info(other.owner_info),
	priv(other.priv ? new Private(*other.priv) : 0)
{ }

Stat &Stat::operator=(const Stat &other)
{
	exists = other.exists;
	type = other.type;
	size = other.size;
	alloc_size = other.alloc_size;
	mtime = other.mtime;
	owner_info = other.owner_info;
	delete priv;
	priv = (other.priv ? new Private(*other.priv) : 0);

	return *this;
}

Stat::~Stat()
{
	delete priv;
}

const string &Stat::get_owner() const
{
	if(priv && owner_info.owner.empty())
		priv->fill_owner_info(owner_info);
	return owner_info.owner;
}

const string &Stat::get_group() const
{
	if(priv && owner_info.group.empty())
		priv->fill_owner_info(owner_info);
	return owner_info.group;
}

} // namespace FS
} // namespace Msp
