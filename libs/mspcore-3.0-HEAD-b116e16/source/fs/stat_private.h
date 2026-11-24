#ifndef MSP_FS_STAT_PRIVATE_H_
#define MSP_FS_STAT_PRIVATE_H_

#include "stat_platform.h"

namespace Msp {
namespace FS {

struct Stat::Private
{
	UserID owner_id;
	GroupID group_id;

	Private();
	Private(const Private &);
	~Private();

#ifndef _WIN32
	/* This is here because it needs access to private members of Stat, but we
	can't expose the system stat struct in the public header */
	static Stat from_struct_stat(const struct stat &);
#endif

	void fill_owner_info(Stat::OwnerInfo &);
};

} // namespace FS
} // namespace Msp

#endif
