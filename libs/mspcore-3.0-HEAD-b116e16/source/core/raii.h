#ifndef MSP_CORE_RAII_H_
#define MSP_CORE_RAII_H_

namespace Msp {

/**
Sets a value for the duration of a scope.  The old value is restored when the
scope ends.
*/
template<typename T>
class SetForScope
{
private:
	T &ref;
	T old;

public:
	SetForScope(T &r, T v): ref(r), old(r) { ref = v; }
	~SetForScope() { ref = old; }
};

class SetFlag: public SetForScope<bool>
{
public:
	SetFlag(bool &r, bool v = true): SetForScope<bool>(r, v) { }
};


template<typename T>
class Conditional
{
private:
	char buf[sizeof(T)];
	T *obj;

public:
	template<typename A>
	Conditional(bool c, const A &a): obj(c ? new(buf) T(a) : 0) { }

	template<typename A0, typename A1>
	Conditional(bool c, const A0 &a0, const A1 &a1): obj(c ? new(buf) T(a0, a1) : 0) { }

	~Conditional() { if(obj) obj->~T(); }
};

} // namespace Msp

#endif
