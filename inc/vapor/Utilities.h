/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//---------------------------------------------------------------------//
// Macro for unused parameters to clean up 'The Warning Disease'
//---------------------------------------------------------------------//

#define VAPOR_UNUSED( id )

#ifndef UNUSED
	#define UNUSED VAPOR_UNUSED
#endif

//---------------------------------------------------------------------//
// Array and Conversion Helpers
//---------------------------------------------------------------------//

#define VAPOR_ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL(N, T)								\
	namespace vapor { namespace N {					\
		class T;									\
	} } // end namespaces

#define FWD_DECL_TYPEDEF_INT(N, T)					\
	namespace vapor { namespace N {					\
		class T;									\
		TYPEDEF_INTRUSIVE_POINTER_FROM_CLASS( T );	\
	} } // end namespaces

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define IMPLEMENT_ACESSOR(name, type, var)		\
	type get##name() const { return var; } 		\
	void set##name(type v) { var = v; }

//-----------------------------------//

// Swaps the endianness of a long.
long endian_swap(long i);

//-----------------------------------//

template< typename T >
T str_to_num(const std::string& s)
{
	std::stringstream ss(s);
	T num; ss >> num; 
	assert(!ss.fail());
	return num;
}

//-----------------------------------//

template< typename T >
std::string num_to_str(const T& t)
{
	std::ostringstream oss;
	oss << t; 
	assert(!oss.fail());
	return oss.str();
}

//-----------------------------------//

std::string wstrtostr(const std::wstring &wstr);

//-----------------------------------//

std::wstring strtowstr(const std::string &str);

//-----------------------------------//

std::vector<std::string>& split(const std::string& s, char delim, 
								std::vector<std::string>& elems);

//-----------------------------------//

std::vector<std::string> split(const std::string& s, char delim);

//-----------------------------------//

} // end namespace