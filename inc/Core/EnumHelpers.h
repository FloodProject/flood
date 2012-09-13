/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include <type_traits>

template<typename T>
T BitwiseFlags(T e1, T e2)
{
	typedef typename std::underlying_type<T>::type UT;
	return static_cast<E>(UT(e1) | UT(e2));
}

template<typename T>
bool BitwiseAnd(T e, T v)
{
	typedef typename std::underlying_type<T>::type UT;
	return (UT(e) & UT(v)) != 0;
}