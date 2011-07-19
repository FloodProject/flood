/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"
#include "Render/Quad.h"

namespace vapor {

//-----------------------------------//

/**
 * Water plane.
 */

REFLECT_DECLARE_CLASS(Water)

class API_SCENE Water : public Geometry
{
public:

	Water();
	Water( MaterialHandle material );

protected:

	// Water plane.
	QuadPtr quad;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Water );

//-----------------------------------//

} // end namespace