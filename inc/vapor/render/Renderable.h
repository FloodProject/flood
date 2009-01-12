/************************************************************************
*
* vaporEngine by triton (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"

namespace vapor {
	namespace render {
		class Device;

	namespace PrimitiveType { // GIMME enum class now!
		enum PrimitiveType
		{
			TRIANGLES,
			LINES
		};
	};

/**
 * Represents a render-able object, that is, the only rendering object
 * the rendering Device knows and cares about. A render-able object has
 * at least one Vertex buffer and one Index buffer, and an primitive 
 * type associated with them.
 */
class Renderable
{

public:

	// Render
	virtual void Render(Device *device) = 0;


private:

	// primitive type of vertex buffer 
	//PrimitiveType _type;
};

} } // end namespaces
