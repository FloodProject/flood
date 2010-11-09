/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

#ifdef VAPOR_PLATFORM_WINDOWS
	typedef struct HINSTANCE__* DynLibHandle;
#else
	#warning "Support for dynamic libraries not found"
#endif

//-----------------------------------//

/**
 * Dynamic library that can be loaded at runtime (DLLs on Windows,
 * SO on *nix, DyLib on MacOS X). After the library is loaded, it is 
 * possible to retrieve symbols using this class.
 */

class VAPOR_API DynamicLibrary
{
	DECLARE_UNCOPYABLE(DynamicLibrary)

public:

	DynamicLibrary( const std::string& name );
	~DynamicLibrary();

	// Gets a pointer to the given symbol.
	void* getSymbol( const std::string& symbol );

private:

	// Loads the dynamic library.
	bool load();
	
	std::string _name;
	DynLibHandle _handle;
};

//-----------------------------------//

} // end namespace