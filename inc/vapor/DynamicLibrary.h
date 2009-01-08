/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace platform {

// based on OGRE 3D headers
#ifdef VAPOR_PLATFORM_WINDOWS
# define DYNLIB_HANDLE				struct HINSTANCE__*
# define DYNLIB_LOAD(lib)			LoadLibraryExA(lib, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
# define DYNLIB_GETSYM(lib, sym)	GetProcAddress(lib, sym)
# define DYNLIB_UNLOAD(lib)			FreeLibrary(lib)
#endif

class DynamicLib
{

public:
	DynamicLib(const string &name);
	~DynamicLib();

	void* getSymbol(const string &symbol);

private:
	bool load();
	
	string _name;
	DYNLIB_HANDLE _handle;
};

} } // end namespaces