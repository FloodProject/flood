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
		TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( T );	\
	} } // end namespaces

#define FWD_DECL_TYPEDEF_SHARED(N, T)				\
	namespace vapor { namespace N {					\
		class T;									\
		TYPEDEF_SHARED_POINTER_FROM_TYPE( T );	\
	} } // end namespaces

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define IMPLEMENT_ACESSOR(name, type, var)		\
	type get##name() const { return var; }		\
	void set##name(type v) { var = v; }

#define IMPLEMENT_ACESSOR_PTR(name, type, var)	\
	type get##name() const { return *var; }		\
	void set##name(type v) { var = &v; }

#define IMPLEMENT_GETTER(name, type, var)		\
	type get##name() const { return var; }

#define IMPLEMENT_SETTER(name, type, var)		\
	void set##name(type v) { var = v; }

//---------------------------------------------------------------------//
// Conversions
//---------------------------------------------------------------------//

bool isLittleEndian();

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

void float_to_str( char* str, float n, byte precision = 2 );

//-----------------------------------//

std::string wstr_to_str(const std::wstring &wstr);
std::wstring str_to_wstr(const std::string &str);

//-----------------------------------//

std::vector<std::string>& str_split(const std::string& s, char delim, 
								std::vector<std::string>& elems);

std::vector<std::string> str_split(const std::string& s, char delim);

//---------------------------------------------------------------------//
// Locales
//---------------------------------------------------------------------//

struct LocaleSaveRestore
{
public:

	LocaleSaveRestore()
	{
		cur = setlocale(LC_NUMERIC, "C");
	}

	~LocaleSaveRestore()
	{
		setlocale(LC_NUMERIC, cur);
	}

private:

	char* cur;
};

//---------------------------------------------------------------------//
// Threading
//---------------------------------------------------------------------//

// Sets the thread name in Windows. Helps with debugging.

//#ifdef VAPOR_PLATFORM_WINDOWS
//
//ulong GetThreadId(HANDLE Thread);
//void SetThreadName( ulong dwThreadID, const std::string& threadName );
//
//#endif

//-----------------------------------//

} // end namespace