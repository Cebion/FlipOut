#include <windows.h>
#include <aclapi.h>
#include <msp/core/systemerror.h>
#include <msp/strings/format.h>
#include <msp/time/rawtime_private.h>
#include "stat.h"
#include "stat_private.h"

using namespace std;

namespace {

PSID copy_sid(PSID sid)
{
	if(!sid || !IsValidSid(sid))
		return 0;
	DWORD len = GetLengthSid(sid);
	PSID copy = reinterpret_cast<PSID>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len));
	if(!CopySid(len, copy, sid))
	{
		DWORD err = GetLastError();
		HeapFree(GetProcessHeap(), 0, copy);
		throw Msp::system_error("CopySid", err);
	}
	return copy;
}

string get_account_name(PSID sid)
{
	char name[1024];
	DWORD nlen = sizeof(name);
	char domain[1024];
	DWORD dlen = sizeof(domain);
	SID_NAME_USE use;
	if(!LookupAccountSid(0, sid, name, &nlen, domain, &dlen, &use))
		throw Msp::system_error("LookupAccountSid");
	return Msp::format("%s/%s", name, domain);
}

}


namespace Msp {
namespace FS {

Stat::Private::Private(const Private &other):
	owner_id(copy_sid(other.owner_id)),
	group_id(copy_sid(other.group_id))
{ }

Stat::Private::~Private()
{
	if(owner_id)
		HeapFree(GetProcessHeap(), 0, owner_id);
	if(group_id)
		HeapFree(GetProcessHeap(), 0, group_id);
}

void Stat::Private::fill_owner_info(Stat::OwnerInfo &result)
{
	if(owner_id)
		result.owner = get_account_name(owner_id);
	else
		result.owner = "None";

	if(group_id)
		result.group = get_account_name(group_id);
	else
		result.group = "None";
}


Stat Stat::stat(const Path &path)
{
	HANDLE handle;
	handle = CreateFile(path.str().c_str(), READ_CONTROL, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS, 0);
	if(handle==INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		if(err==ERROR_FILE_NOT_FOUND || err==ERROR_PATH_NOT_FOUND)
			return Stat();
		else
			throw system_error(format("CreateFile(%s)", path), err);
	}

	BY_HANDLE_FILE_INFORMATION info;
	if(!GetFileInformationByHandle(handle, &info))
	{
		CloseHandle(handle);
		throw system_error("GetFileInformationByHandle");
	}

	Stat result;
	result.exists = true;
	if(info.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		result.type = DIRECTORY;
	else
		result.type = REGULAR;

	result.size = FileSize(info.nFileSizeHigh)<<32 | info.nFileSizeLow;
	result.alloc_size = (result.size+511)&~511;
	result.mtime = Time::TimeStamp(Time::filetime_to_rawtime(info.ftLastWriteTime));

	PSECURITY_DESCRIPTOR sec_desc;
	PSID owner = 0;
	PSID group = 0;
	const SECURITY_INFORMATION sec_info = OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION;
	DWORD err = GetSecurityInfo(handle, SE_FILE_OBJECT, sec_info, &owner, &group, 0, 0, &sec_desc);
	if(err)
	{
		CloseHandle(handle);
		throw system_error("GetSecurityInfo", err);
	}

	result.priv = new Private;
	result.priv->owner_id = copy_sid(owner);
	result.priv->group_id = copy_sid(group);

	LocalFree(sec_desc);

	CloseHandle(handle);

	return result;
}

Stat Stat::lstat(const Path &path)
{
	return stat(path);
}

bool exists(const Path &path)
{
	return GetFileAttributes(path.str().c_str())!=INVALID_FILE_ATTRIBUTES;
}

} // namespace FS
} // namespace Msp
