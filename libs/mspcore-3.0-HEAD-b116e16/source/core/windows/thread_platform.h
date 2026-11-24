#ifndef MSP_CORE_THREAD_PLATFORM_H_
#define MSP_CORE_THREAD_PLATFORM_H_

#include <windows.h>

namespace Msp {

typedef HANDLE ThreadHandle;
typedef DWORD ThreadReturn;

#define THREAD_CALL WINAPI

} // namespace Msp

#endif
