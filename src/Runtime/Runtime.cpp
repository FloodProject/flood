/************************************************************************
*
* Flood Project © (2008-201x) 
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Runtime/API.h"
#include "Runtime/Runtime.h"

FLUSH_NAMESPACE_BEGIN

//-----------------------------------//

Runtime::Runtime()
{

}

//-----------------------------------//

void Runtime::onInit()
{
	// Initialize the Mono scripting runtime.
	monoRuntime.initialize();
}

//-----------------------------------//

void Runtime::onCleanup()
{
	// Cleanup the Mono scripting runtime.
	monoRuntime.cleanup();
}

//-----------------------------------//

FLUSH_NAMESPACE_END