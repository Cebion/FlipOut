#ifndef MSP_CORE_ATTRIBUTES_H_
#define MSP_CORE_ATTRIBUTES_H_

#if __cplusplus >= 201402L
#define DEPRECATED [[deprecated]]
#elif defined(__GNUC__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#endif

#endif
