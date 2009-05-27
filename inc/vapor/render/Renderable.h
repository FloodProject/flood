/************************************************************************
*
* vaporEngine by triton (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"

namespace vapor {
	namespace render {

/**
 * Type of primitive of the renderable.
 */

namespace PrimitiveType
{
	enum Enum
	{
		Triangles,
		Lines
	};
}

/**
 * Represents a renderable object, that is, the only rendering object
 * the rendering Device knows and cares about. A render-able object has
 * at least one Vertex buffer and one Index buffer, and an primitive 
 * type associated with them.
 */

class Renderable
{
public:

	// Render
	//virtual void Render(Device *device) = 0;

private:

	// primitive type of vertex buffer 
	PrimitiveType::Enum type;
};

} } // end namespaces
