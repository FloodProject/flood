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

class RESOURCE_API Text : public Resource
{
public:

	// Gets the text.
	ACESSOR(Text, const std::string&, text)

	virtual void parse() {}

	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::General)

protected:

	// Text content.
	std::string text;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Text );

//-----------------------------------//

} // end namespace

//#endif