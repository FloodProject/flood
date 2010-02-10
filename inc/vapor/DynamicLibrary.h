/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace platform {

//-----------------------------------//

/**
 * We set some macros up-front for all the platform dynamic-lib related 
 * functions. This makes the class code more clean, since otherwise, the
 * code would be ridden with platform-specific conditional blocks.
 */

// based on OGRE 3D headers
#ifdef VAPOR_PLATFORM_WINDOWS
	#define DYNLIB_HANDLE			struct HINSTANCE__*
	#define DYNLIB_LOAD(lib)		LoadLibraryExA(lib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH)
	#define DYNLIB_GETSYM(lib, sym)	GetProcAddress(lib, sym)
	#define DYNLIB_UNLOAD(lib)		FreeLibrary(lib)
#else
	#error "Support for dynamic libraries not found"
#endif

//-----------------------------------//

/**
 * Dynamic library that can be loaded at runtime (DLLs on Windows,
 * SO on *nix, DyLib on MacOS X). After the library is loaded, it is 
 * possible to retrieve symbols using this class.
 */

class VAPOR_API DynamicLib : private boost::noncopyable
{
public:

	DynamicLib( const std::string& name );
	~DynamicLib();

	// Gets a pointer to the given symbol
	void* getSymbol( const std::string& symbol );

private:

	bool load();
	
	std::string _name;
	DYNLIB_HANDLE _handle;
};

//-----------------------------------//

} } // end namespaces