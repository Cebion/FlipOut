#include <grp.h>
#include <pwd.h>
#include <msp/strings/format.h>
#include "stat.h"
#include "stat_private.h"

namespace Msp {
namespace FS {

void Stat::Private::fill_owner_info(Stat::OwnerInfo &result)
{
	char buf[1024];

	struct passwd pw;
	struct passwd *owner;
	if(!getpwuid_r(owner_id, &pw, buf, sizeof(buf), &owner) && owner)
		result.owner = owner->pw_name;
	else
		result.owner = format("%d", owner_id);

	struct group gr;
	struct group *group;
	if(!getgrgid_r(group_id, &gr, buf, sizeof(buf), &group) && group)
		result.group = group->gr_name;
	else
		result.group = format("%d", group_id);
}

} // namespace FS
} // namespace Msp
