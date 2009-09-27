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

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Vertex buffer with a quad.
 */

class VAPOR_API Quad : public render::Renderable
{
public:

	Quad(  MaterialPtr mat = MaterialPtr(), IndexBufferPtr ib = IndexBufferPtr() );

	virtual ~Quad();

private:

	render::VertexBufferPtr getQuadBuffer();
};

//-----------------------------------//

typedef tr1::shared_ptr< Quad > QuadPtr;

//-----------------------------------//

} } // end namespaces