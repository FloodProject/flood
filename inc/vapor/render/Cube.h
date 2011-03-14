/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Renderable.h"

namespace vapor {

//-----------------------------------//

/**
 * Generates a cube.
 */

class VAPOR_API Cube : public Renderable
{
public:

	Cube( float width = 1.0f, float height = 1.0f );

	static VertexBufferPtr buildGeometry( float width, float height );
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Cube );

//-----------------------------------//

} // end namespace