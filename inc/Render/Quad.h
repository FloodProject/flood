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
 * Vertex buffer with a quad.
 */

class VAPOR_API Quad : public Renderable
{
public:

	Quad( float width = 1.0f, float height = 1.0f );

private:

	VertexBufferPtr getQuadBuffer( float width, float height );
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Quad );

//-----------------------------------//

} // end namespace