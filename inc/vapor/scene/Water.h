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

class VAPOR_API Water : public Geometry
{
	DECLARE_CLASS_()

public:

	Water();
	Water( MaterialPtr material );

protected:

	// Water plane.
	QuadPtr quad;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Water );

//-----------------------------------//

} // end namespace