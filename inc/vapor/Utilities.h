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
// Helper macros to stringify parameters.
//---------------------------------------------------------------------//

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define __FILE__LINE__ __FILE__ "(" TOSTRING(__LINE__) ") : "
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x "\n" ) 

//---------------------------------------------------------------------//
// Array and Conversion Helpers
//---------------------------------------------------------------------//

#define VAPOR_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

//---------------------------------------------------------------------//
// Forward-declaration Helpers
//---------------------------------------------------------------------//

#define FWD_DECL_INTRUSIVE(T)						\
	namespace vapor {								\
		class T;									\
		TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( T );	\
	} // end namespace
	
#define FWD_DECL_SHARED(T)							\
	namespace vapor {								\
		class T;									\
		TYPEDEF_SHARED_POINTER_FROM_TYPE( T );		\
	} // end namespace

#define FWD_DECL_SHARED_WEAK(T)						\
	namespace vapor {								\
		class T;									\
		TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( T );	\
	} // end namespace

//---------------------------------------------------------------------//
// Acessors
//---------------------------------------------------------------------//

#define DECLARE_GETTER(name, type)				\
	type get##name() const;

#define DECLARE_SETTER(name, type)				\
	void set##name(type v);

#define DECLARE_ACESSOR(name, type)				\
	DECLARE_GETTER(name, type)					\
	DECLARE_SETTER(name, type)

#define GETTER(name, type, var)					\
	type get##name() const { return var; }

#define SETTER(name, type, var)					\
	void set##name(type v) { var = v; }

#define GETTER_PTR(name, type, var)				\
	type get##name() const { return *var; }

#define SETTER_PTR(name, type, var)				\
	void set##name(type v) { var = &v; }

#define ACESSOR(name, type, var)				\
	GETTER(name, type, var)						\
	SETTER(name, type, var)

#define ACESSOR_PTR(name, type, var)			\
	GETTER_PTR(name, type, var)					\
	SETTER_PTR(name, type, var)

#define STATIC_ACESSOR(name, type, var)			\
	static type get##name() { return var; }		\
	static SETTER(name, type, var)

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

	// Converts a number to a string.
	template< typename T >
	std::string fromNumber(const T& t)
	{
		std::ostringstream oss;
		
		oss << t;
		assert( !oss.fail() );
		
		return oss.str();
	}

	// Converts a string to a number.
	template< typename T >
	T toNumber(const std::string& s)
	{
		std::stringstream ss(s);
		
		T num;
		ss >> num; 
		assert( !ss.fail() );

		return num;
	}

	// Converts a float to a string.
	void fromFloat( char* str, float n, byte precision = 2 );
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

//-----------------------------------//

} // end namespace