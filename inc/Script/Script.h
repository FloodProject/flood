/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Script that can be executed by a script state.
 */

API_ENGINE REFLECT_DECLARE_CLASS(Script)

class API_ENGINE Script : public Resource
{
	REFLECT_DECLARE_OBJECT(Script)

public:

	// Gets the source code of the script.
	ACESSOR(Source, const String&, text)

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Scripts)

	// Source code of the script.
	String text;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Script );

//-----------------------------------//

NAMESPACE_ENGINE_END