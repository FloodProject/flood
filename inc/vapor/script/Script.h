/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/resources/Resource.h"

namespace vapor { namespace script {

class State;

//-----------------------------------//

/**
 * Scripting context that can be used to execute scripts. Currently this
 * represents a Lua interpreter state.
 */

class VAPOR_API Script : public Resource
{
public:

	// Execute the script.
	void execute();

	// Gets the source code of the script.
	IMPLEMENT_ACESSOR(Source, const std::string&, text)

	// Sets the state where the script will execute.
	IMPLEMENT_ACESSOR(State, State* const, state)

	// Gets the associated resource group.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum,
		ResourceGroup::Scripts)

protected:

	// Source code of the script.
	std::string text;

	// State where the script will execute.
	State* state;
};

//-----------------------------------//

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} } // end namespaces

//#endif