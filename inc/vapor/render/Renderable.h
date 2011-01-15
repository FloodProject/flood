/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "render/VertexBuffer.h"
#include "render/IndexBuffer.h"
#include "render/Material.h"
#include "render/GL.h"
#include "Event.h"

namespace vapor {

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 */

namespace PolygonType
{
    enum Enum
    {
        Points			= GL_POINTS,
        Lines			= GL_LINES,
        LineLoop		= GL_LINE_LOOP,
        LineStrip		= GL_LINE_STRIP,
        Triangles		= GL_TRIANGLES,
        TriangleStrip	= GL_TRIANGLE_STRIP,
        TriangleFan		= GL_TRIANGLE_FAN,
        Quads			= GL_QUADS,
        QuadStrip		= GL_QUAD_STRIP,
        Polygon			= GL_POLYGON
    };
}

//-----------------------------------//

/**
 * Type of rendering mode of the renderable.
 */

namespace PolygonMode
{
    enum Enum
    {
		Solid		= GL_FILL,
		Wireframe	= GL_LINE,
		Point		= GL_POINT
    };
}

//-----------------------------------//

class RenderDevice;

/**
 * Represents a renderable object, that is, the only rendering object
 * the rendering device knows and cares about. A renderable object has
 * at least one vertex buffer and one index buffer, and a primitive 
 * type associated with them.
 */

class VAPOR_API Renderable : public ReferenceCounted
{
public:

	Renderable();
    Renderable( PolygonType::Enum );
    
	// Binds the state needed to draw the renderable.
	bool bind();

	// Unbinds the state needed to draw the renderable.
	bool unbind();

    // Render this renderable.
	void render(RenderDevice* device);
    
    // Gets/sets the vertex buffer associated with the renderable.
	ACESSOR(VertexBuffer, VertexBufferPtr, vb) 
   
    // Gets/sets the index buffer associated with the renderable.
	ACESSOR(IndexBuffer, IndexBufferPtr, ib)

    // Gets/sets the material associated with the renderable.
    ACESSOR(Material, MaterialPtr, material)

    // Gets/sets the render mode of this renderable.
	ACESSOR(PolygonMode, PolygonMode::Enum, mode)

	// Gets/sets the primitive type of this renderable.
	ACESSOR(PrimitiveType, PolygonType::Enum, type)

	// Pre-render callback.
	Event0<> onPreRender;

	// Post-render callback.
	Event0<> onPostRender;

protected:

    // Primitive type of the vertex buffer contents.
    PolygonType::Enum type;

	// Rendering mode of the vertex buffer contents.
	PolygonMode::Enum mode;

	// Vertex buffer with all the vertices.
    VertexBufferPtr vb;

	// Index buffer with all the indices.
    IndexBufferPtr ib;
    
	// Material of this renderable.
	MaterialPtr material;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Renderable );

//-----------------------------------//

} // end namespace