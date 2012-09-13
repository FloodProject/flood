/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Runtime/API.h"
#include "Runtime/Mono.h"

NAMESPACE_RUNTIME_BEGIN

//-----------------------------------//

/**
 * This runtime is a standardized approach to the distribution of 
 * applications. As long as the application sticks to the defined API
 * then it can be launched and managed by the runtime automatically.
 *
 * It's responsible for loading all the program modules, aswell as
 * registering all the core services available to the scripting layer.
 */

class API_RUNTIME Runtime
{
public:

	Runtime();
	virtual ~Runtime() {}

	// Initialize the runtime.
	void init();
	void shutdown();

	MonoRuntime monoRuntime;
};

//-----------------------------------//

NAMESPACE_RUNTIME_END
