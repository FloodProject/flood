/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Runtime/API.h"
#include "Runtime/Mono.h"

FLUSH_NAMESPACE_BEGIN

//-----------------------------------//

/**
 * This runtime is a standardized approach to the distribution of 
 * applications. As long as the application sticks to the defined API
 * then it can be launched and managed by the runtime automatically.
 *
 * It's responsible for loading all the program modules, aswell as
 * registering all the core services available to the scripting layer.
 */

class Runtime
{
public:

	Runtime();

	// Initialize the runtime.
	void onInit();
	void onCleanup();

	MonoRuntime monoRuntime;
};

//-----------------------------------//

FLUSH_NAMESPACE_END