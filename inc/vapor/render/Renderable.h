/************************************************************************
*
* vaporEngine by triton (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/Material.h"
#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"

#include "vapor/math/Matrix4x3.h"

#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

class Device;

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 */

namespace Primitive
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

/** TODO: move this to material */

namespace RenderMode
{
    enum Enum
    {
		Solid = GL_FILL,
		Wireframe = GL_LINE
    };
}

//-----------------------------------//

/**
 * Represents a renderable object, that is, the only rendering object
 * the rendering Device knows and cares about. A render-able object has
 * at least one Vertex buffer and one Index buffer, and an primitive 
 * type associated with them.
 */

class VAPOR_API Renderable
{
public:

	Renderable();

    Renderable( Primitive::Enum primitive, 
		VertexBufferPtr vb, IndexBufferPtr ib, MaterialPtr mat );
    
    // No index buffer, default material will be used if none passed
	Renderable(Primitive::Enum primitive, VertexBufferPtr vb = VertexBufferPtr(),
			MaterialPtr mat = MaterialPtr( ) );

	virtual ~Renderable();
                    
    // Render this renderable. This will bind all the necessary state
	// like binding the buffers and the materials.
	void render(render::Device& device);
    
    // Gets the vertex buffer associated with the renderable.
    VertexBufferPtr getVertexBuffer() const;

    // Gets the index buffer associated with the renderable.
    IndexBufferPtr getIndexBuffer() const;    

    // Gets the material associated with the renderable.
    MaterialPtr getMaterial() const;

    // Gets the render mode of this renderable.
    RenderMode::Enum getRenderMode() const;

	// Sets the vertex buffer of this renderable.
	void setVertexBuffer( VertexBufferPtr vb );

	// Sets the index buffer of this renderable.
	void setIndexBuffer( IndexBufferPtr ib );

	// Sets this renderable with a new material.
	void setMaterial( MaterialPtr mat );

	// Sets the primitive type of this renderable.
	void setPrimitiveType( Primitive::Enum p );

	// Sets the render mode of this renderable.
	void setRenderMode( RenderMode::Enum mode );

protected:

    // Primitive type of the vertex buffer contents.
    Primitive::Enum type;

	// Rendering mode of the vertex buffer contents.
	RenderMode::Enum mode;

	// Vertex buffer with all the vertices.
    VertexBufferPtr vb;

	// Index buffer with all the indices.
    IndexBufferPtr ib;
    
	// Material of this renderable.
	MaterialPtr mat;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Renderable );

//-----------------------------------//

} } // end namespaces