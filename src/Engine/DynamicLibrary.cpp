/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/DynamicLibrary.h"

namespace vapor {
	namespace platform {

DynamicLib::DynamicLib(const std::string& name) 
	: _handle(nullptr), _name(name)
{
	load();
}

DynamicLib::~DynamicLib()
{
	if(_handle) {
		DYNLIB_UNLOAD(_handle);
	}
}

bool DynamicLib::load()
{
	_handle = (DYNLIB_HANDLE) DYNLIB_LOAD(_name.c_str());

	if(!_handle) {
		return false;
	}

	return true;
}

void* DynamicLib::getSymbol(const std::string& symbol)
{
	return (void*) DYNLIB_GETSYM(_handle, symbol.c_str()); 
}

} } // end namespaces