/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/Renderable.h"
#include "vapor/render/Material.h"

#include "vapor/math/Dimension.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Vertex buffer with a quad.
 */

class VAPOR_API Quad : public render::Renderable
{
public:

	Quad( const math::Dimension& dim = math::Dimension( 1, 1 ),
		MaterialPtr mat = MaterialPtr(), 
		IndexBufferPtr ib = IndexBufferPtr() );

	virtual ~Quad();

private:

	render::VertexBufferPtr getQuadBuffer( const math::Dimension& dim );
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Quad );

//-----------------------------------//

} } // end namespaces