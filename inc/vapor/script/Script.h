/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "resources/Resource.h"

namespace vapor {

//-----------------------------------//

class State;

/**
 * Script that can be executed by a script state.
 */

class VAPOR_API Script : public Resource
{
	DECLARE_CLASS_()

public:

	// Gets the source code of the script.
	ACESSOR(Source, const std::string&, text)

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Scripts)

protected:

	// Source code of the script.
	std::string text;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Script );

//-----------------------------------//

} // end namespace