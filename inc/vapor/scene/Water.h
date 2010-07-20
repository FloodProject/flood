/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/render/Quad.h"

namespace vapor {

//-----------------------------------//

/**
 * Water plane.
 */

class VAPOR_API Water : public Geometry
{
public:

	Water( MaterialPtr material );

	// Returns the name of this component.
	GETTER(Type, const std::string&, type)

protected:

	// Water plane.
	QuadPtr quad;

	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Water );

//-----------------------------------//

} // end namespace