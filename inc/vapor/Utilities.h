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
// System Information
//---------------------------------------------------------------------//

namespace System
{
	// Returns if the system is little-endian.
	bool isLittleEndian();

	// Swaps the endianness of a long.
	long swapEndian(long i);
}

//---------------------------------------------------------------------//
// Conversions
//---------------------------------------------------------------------//

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

namespace String
{
	// Formats the string according to the 'printf' style format string.
	// Note: This is not type-safe so be careful.
	VAPOR_API std::string format(const char* str, ...);
	VAPOR_API std::string format(const char* str, va_list args);

	// Splits a string (tokenizes) into sub-string separated by given elements.
	VAPOR_API std::vector<std::string>& split(const std::string& s, char delim, 
								std::vector<std::string>& elems);

	// Splits a string (tokenizes) into sub-string separated by given elements.
	VAPOR_API std::vector<std::string> split(const std::string& s, char delim);

	// Converts a wide string to a regular string.
	VAPOR_API std::string fromWideString(const std::wstring& wstr);
	
	// Converts a regular string into a wide string.
	VAPOR_API std::wstring toWideString(const std::string& str);

	// Converts a regular string to lower case.
	VAPOR_API std::string toLowerCase(const std::string& str);

	// Converts a regular string to upper case.
	VAPOR_API std::string toUpperCase(const std::string& str);
}

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