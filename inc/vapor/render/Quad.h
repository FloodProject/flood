/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"
#include "vapor/render/Material.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Vertex buffer with a quad.
 */

class VAPOR_API Quad : public render::Renderable
{
public:

	Quad( float width = 1.0f, float height = 1.0f );

private:

	render::VertexBufferPtr getQuadBuffer( float width, float height );
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Quad );

//-----------------------------------//

} } // end namespaces