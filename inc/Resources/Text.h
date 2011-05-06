/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"

namespace vapor {

//-----------------------------------//

/**
 * Textual content resource that can be used as a base for all text-based
 * resource types, like scripts, shaders, materials, scenes, etc.
 */

REFLECT_DECLARE_CLASS(Text)

class RESOURCE_API Text : public Resource
{
	REFLECT_DECLARE_OBJECT(Text)

public:

	// Gets the text.
	ACESSOR(Text, const String&, text)

	virtual void parse() {}

	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::General)

protected:

	// Text content.
	String text;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Text );

//-----------------------------------//

} // end namespace

//#endif