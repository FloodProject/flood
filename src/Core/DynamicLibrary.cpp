/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/DynamicLibrary.h"

#ifdef ENABLE_DYNAMIC_LIBRARY

#ifdef PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>	
#endif

#ifdef PLATFORM_WINDOWS
	#define DYNLIB_LOAD(lib)		LoadLibraryExA(lib, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH)
	#define DYNLIB_GETSYM(lib, sym)	GetProcAddress((HINSTANCE) lib, sym)
	#define DYNLIB_UNLOAD(lib)		FreeLibrary((HINSTANCE) lib)
#else
	#error "Support for dynamic libraries not found"
#endif

NAMESPACE_CORE_BEGIN

//-----------------------------------//

DynamicLibrary::DynamicLibrary(const String& name) 
  : _handle(nullptr)
  , _name(name)
{
	load();
}

//-----------------------------------//

DynamicLibrary::~DynamicLibrary()
{
	if(_handle)
		DYNLIB_UNLOAD(_handle);
}

//-----------------------------------//

bool DynamicLibrary::load()
{
	_handle = (DynLibHandle) DYNLIB_LOAD(_name.CString());
	return _handle != 0;
}

//-----------------------------------//

void* DynamicLibrary::getSymbol(const String& symbol)
{
	return (void*) DYNLIB_GETSYM(_handle, symbol.CString()); 
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif
