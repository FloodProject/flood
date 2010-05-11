/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Resource.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * Textual content resource that can be used as a base for all text-based
 * resource types, like scripts, shaders, materials, scenes, etc.
 */

class VAPOR_API Text : public Resource
{
public:

	// Gets the text.
	IMPLEMENT_ACESSOR(Text, const std::string&, text)

	virtual void parse() {}

	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::General)

protected:

	// Text content.
	std::string text;
};

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( Text );

//-----------------------------------//

} } // end namespaces

//#endif