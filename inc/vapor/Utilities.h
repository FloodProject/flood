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
// Macro for unused parameters to clean up compiler warnings
//---------------------------------------------------------------------//

#define VAPOR_UNUSED( id )

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

#define FWD_DECL_TYPEDEF_PTR(T)						\
	namespace vapor {								\
		class T;									\
		TYPEDEF_PTR( T );							\
	} // end namespace

#define FWD_DECL_NS_TYPEDEF_PTR(N, T)				\
	namespace vapor { namespace N {					\
		class T;									\
		TYPEDEF_PTR( T );							\
	} } // end namespaces

#define FWD_DECL_NS_TYPEDEF_INT(N, T)				\
	namespace vapor { namespace N {					\
		class T;									\
		TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( T );	\
	} } // end namespaces

#define FWD_DECL_NS_TYPEDEF_SHARED(N, T)			\
	namespace vapor { namespace N {					\
		class T;									\
		TYPEDEF_SHARED_POINTER_FROM_TYPE( T );		\
	} } // end namespaces

#define FWD_DECL_NS_TYPEDEF_SHARED_WEAK(N, T)			\
	namespace vapor { namespace N {						\
		class T;										\
		TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( T );		\
	} } // end namespaces

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define DECLARE_GETTER(name, type)	\
	type get##name() const;

#define DECLARE_SETTER(name, type)	\
	void set##name(type v);

#define DECLARE_ACESSOR(name, type)	\
	DECLARE_GETTER(name, type)		\
	DECLARE_SETTER(name, type)

#define IMPLEMENT_GETTER(name, type, var)		\
	type get##name() const { return var; }

#define IMPLEMENT_SETTER(name, type, var)		\
	void set##name(type v) { var = v; }

#define IMPLEMENT_GETTER_PTR(name, type, var)	\
	type get##name() const { return *var; }

#define IMPLEMENT_SETTER_PTR(name, type, var)	\
	void set##name(type v) { var = &v; }

#define IMPLEMENT_ACESSOR(name, type, var)		\
	IMPLEMENT_GETTER(name, type, var)			\
	IMPLEMENT_SETTER(name, type, var)

#define IMPLEMENT_ACESSOR_PTR(name, type, var)	\
	IMPLEMENT_GETTER_PTR(name, type, var)		\
	IMPLEMENT_SETTER_PTR(name, type, var)

#define IMPLEMENT_STATIC_ACESSOR(name, type, var)	\
	static type get##name() { return var; }			\
	static IMPLEMENT_SETTER(name, type, var)

//---------------------------------------------------------------------//
// Conversions
//---------------------------------------------------------------------//

// Returns if the system is little-endian.
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