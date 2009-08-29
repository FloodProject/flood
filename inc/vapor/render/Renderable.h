/************************************************************************
*
* vaporEngine by triton (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

//#include "vapor/render/Device.h"
//#include "vapor/render/Material.h"
//#include "vapor/render/VertexBuffer.h"
//#include "vapor/render/IndexBuffer.h"

#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

class Device;
class Material;
class VertexBuffer;
class IndexBuffer;

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 * 
 */

namespace PrimitiveType
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

    Renderable(PrimitiveType::Enum primitive, tr1::shared_ptr< VertexBuffer > vb, 
					tr1::shared_ptr< IndexBuffer > ib, tr1::shared_ptr< Material >);
    
    // No index buffer
	Renderable(PrimitiveType::Enum primitive, tr1::shared_ptr< VertexBuffer > vb,
					tr1::shared_ptr< Material >);
    
    // No index buffer, default material will be used
    Renderable(PrimitiveType::Enum primitive, tr1::shared_ptr< VertexBuffer > vb);
                    
    // Render this renderable. This will bind all the necessary state like binding
    // the buffers and the materials.
	void render(render::Device& device) const;
    
    // Gets the vertex buffer associated with the renderable.
    tr1::shared_ptr<VertexBuffer> getVertexBuffer() const;

    // Gets the index buffer associated with the renderable.
    tr1::shared_ptr<IndexBuffer> getIndexBuffer() const;    

    // Gets the material associated with the renderable.
    tr1::shared_ptr<Material> getMaterial() const;

private:

    // primitive type of vertex buffer 
    PrimitiveType::Enum type;

    tr1::shared_ptr< VertexBuffer > vertexBuffer;
    tr1::shared_ptr< IndexBuffer > indexBuffer;
    tr1::shared_ptr< Material > material;
};

//-----------------------------------//

} } // end namespaces