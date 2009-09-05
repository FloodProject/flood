/************************************************************************
*
* vaporEngine by triton (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/Device.h"
#include "vapor/render/Material.h"
#include "vapor/render/VertexBuffer.h"
#include "vapor/render/IndexBuffer.h"

#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 * 
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

/**
 * Represents a renderable object, that is, the only rendering object
 * the rendering Device knows and cares about. A render-able object has
 * at least one Vertex buffer and one Index buffer, and an primitive 
 * type associated with them.
 */

class Renderable
{
public:

    Renderable( Primitive::Enum primitive, 
		VertexBufferPtr vb, IndexBufferPtr ib, MaterialPtr mat );
    
    // No index buffer, default material will be used if none passed
	Renderable(Primitive::Enum primitive, VertexBufferPtr vb,
			MaterialPtr mat = MaterialPtr( ) );   
                    
    // Render this renderable. This will bind all the necessary state like binding
    // the buffers and the materials.
	void render(render::Device& device);
    
    // Gets the vertex buffer associated with the renderable.
    VertexBufferPtr getVertexBuffer() const;

    // Gets the index buffer associated with the renderable.
    IndexBufferPtr getIndexBuffer() const;    

    // Gets the material associated with the renderable.
    MaterialPtr getMaterial() const;

private:

    // primitive type of vertex buffer 
    Primitive::Enum type;

    VertexBufferPtr vb;
    IndexBufferPtr ib;
    MaterialPtr mat;
};

//-----------------------------------//

typedef tr1::shared_ptr< Renderable > RenderablePtr;

//-----------------------------------//

// This is a queue of objects that are usually returned by performing a culling
// operation on the camera.
typedef std::list< 
	std::pair< shared_ptr< render::Renderable >, math::Matrix4*> > RenderQueue;

//-----------------------------------//

} } // end namespaces