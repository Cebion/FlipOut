namespace Msp {

struct Null { };

template<typename H, typename T>
struct TypeList
{
	typedef H Head;
	typedef T Tail;
};

template<typename T, unsigned S, bool C>
struct TypeChooser;

template<typename T, unsigned S>
struct TypeChooser<T, S, true>
{
	typedef typename T::Head Type;
};

template<typename T, unsigned S>
struct TypeChooser<T, S, false>
{
	typedef typename TypeChooser<typename T::Tail, S>::Type Type;
};

} // namespace Msp
