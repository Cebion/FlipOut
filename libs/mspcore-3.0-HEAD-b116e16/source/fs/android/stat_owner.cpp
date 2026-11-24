#include <grp.h>
#include <pwd.h>
#include <msp/strings/format.h>
#include "stat.h"
#include "stat_private.h"

namespace Msp {
namespace FS {

void Stat::Private::fill_owner_info(Stat::OwnerInfo &result)
{
	struct passwd *owner;
	if((owner = getpwuid(owner_id)))
		result.owner = owner->pw_name;
	else
		result.owner = format("%d", owner_id);

	struct group *group;
	if((group = getgrgid(group_id)))
		result.group = group->gr_name;
	else
		result.group = format("%d", group_id);
}

} // namespace FS
} // namespace Msp
