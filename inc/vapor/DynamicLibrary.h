/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

#ifdef VAPOR_PLATFORM_WINDOWS
	typedef struct HINSTANCE__* DynLibHandle;
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
	DynLibHandle _handle;
};

//-----------------------------------//

} // end namespace