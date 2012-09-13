/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Textual content resource that can be used as a base for all text-based
 * resource types, like scripts, shaders, materials, scenes, etc.
 */

API_ENGINE REFLECT_DECLARE_CLASS(Text)

class API_ENGINE Text : public Resource
{
	REFLECT_DECLARE_OBJECT(Text)

public:

	// Gets the text.
	ACCESSOR(Text, const String&, text)

	virtual void parse() {}

	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::General)

protected:

	// Text content.
	String text;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Text );

//-----------------------------------//

NAMESPACE_ENGINE_END

//#endif