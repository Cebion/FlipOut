#ifndef MSP_CORE_NONCOPYABLE_H_
#define MSP_CORE_NONCOPYABLE_H_

namespace Msp {

class NonCopyable
{
protected:
	NonCopyable() = default;
	NonCopyable(const NonCopyable &) = delete;
	NonCopyable &operator=(const NonCopyable &) = delete;
	~NonCopyable() = default;
};

} // namespace Msp

#endif
