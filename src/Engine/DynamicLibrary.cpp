/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/DynamicLibrary.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define DYNLIB_LOAD(lib)		LoadLibraryExA(lib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH)
	#define DYNLIB_GETSYM(lib, sym)	GetProcAddress(lib, sym)
	#define DYNLIB_UNLOAD(lib)		FreeLibrary(lib)
#else
	#error "Support for dynamic libraries not found"
#endif

namespace vapor {

//-----------------------------------//

DynamicLib::DynamicLib(const std::string& name) 
  : _handle(nullptr),
	_name(name)
{
	load();
}

//-----------------------------------//

DynamicLib::~DynamicLib()
{
	if(_handle)
	{
		DYNLIB_UNLOAD(_handle);
	}
}

//-----------------------------------//

bool DynamicLib::load()
{
	_handle = (DynLibHandle) DYNLIB_LOAD(_name.c_str());

	if(!_handle) {
		return false;
	}

	return true;
}

//-----------------------------------//

void* DynamicLib::getSymbol(const std::string& symbol)
{
	return (void*) DYNLIB_GETSYM(_handle, symbol.c_str()); 
}

//-----------------------------------//

} // end namespace