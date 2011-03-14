/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_DYNAMIC_LIBRARY

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	typedef void* DynLibHandle;
#else
	#error "Support for dynamic libraries not found"
#endif

//-----------------------------------//

/**
 * Dynamic library that can be loaded at runtime (DLLs on Windows,
 * SO on *nix, DyLib on MacOS X). After the library is loaded, it is 
 * possible to retrieve symbols using this class.
 */

class CORE_API DynamicLibrary
{
	DECLARE_UNCOPYABLE(DynamicLibrary)

public:

	DynamicLibrary( const String& name );
	~DynamicLibrary();

	// Gets a pointer to the given symbol.
	void* getSymbol( const String& symbol );

private:

	// Loads the dynamic library.
	bool load();
	
	String _name;
	DynLibHandle _handle;
};

//-----------------------------------//

} // end namespace

#endif