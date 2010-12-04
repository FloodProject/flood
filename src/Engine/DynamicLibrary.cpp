/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "DynamicLibrary.h"

#ifdef VAPOR_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

#ifdef VAPOR_PLATFORM_WINDOWS
	#define DYNLIB_LOAD(lib)		LoadLibraryExA(lib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH)
	#define DYNLIB_GETSYM(lib, sym)	GetProcAddress((HINSTANCE) lib, sym)
	#define DYNLIB_UNLOAD(lib)		FreeLibrary((HINSTANCE) lib)
#else
	#error "Support for dynamic libraries not found"
#endif

namespace vapor {

//-----------------------------------//

DynamicLibrary::DynamicLibrary(const std::string& name) 
  : _handle(nullptr)
  , _name(name)
{
	load();
}

//-----------------------------------//

DynamicLibrary::~DynamicLibrary()
{
	if(_handle)
	{
		DYNLIB_UNLOAD(_handle);
	}
}

//-----------------------------------//

bool DynamicLibrary::load()
{
	_handle = (DynLibHandle) DYNLIB_LOAD(_name.c_str());

	if(!_handle) {
		return false;
	}

	return true;
}

//-----------------------------------//

void* DynamicLibrary::getSymbol(const std::string& symbol)
{
	return (void*) DYNLIB_GETSYM(_handle, symbol.c_str()); 
}

//-----------------------------------//

} // end namespace