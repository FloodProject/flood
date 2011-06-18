/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Delegate.h"
#include "Resources/Material.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"

FWD_DECL_INTRUSIVE(Material)

#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON 0x0009

#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02

namespace vapor {

//-----------------------------------//

struct RenderState;
	
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

/**
 * Use these different kinds of render groups to signal to the renderer
 * how you want your renderables to be sorted by the render device.
 * Lower numbers render before higher numbers.
 */

namespace RenderLayer
{
	enum Enum
	{
		Normal = 0,
		Transparency = 5,
		PostTransparency = 7,
		Overlays = 10
	};
}

//-----------------------------------//

class RenderDevice;

/**
 * Represents a renderable object, that contains the details the
 * rendering device needs to render the object. At the least, it
 * should have a material and a vertex buffer.
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

    // Gets/sets the render stage.
	ACESSOR(RenderLayer, RenderLayer::Enum, stage)
   
    // Gets/sets the render priority.
	ACESSOR(RenderPriority, int32, priority)
    
    // Gets/sets the vertex buffer.
	ACESSOR(VertexBuffer, VertexBufferPtr, vb)
   
    // Gets/sets the index buffer.
	ACESSOR(IndexBuffer, IndexBufferPtr, ib)

    // Gets/sets the material.
    ACESSOR(Material, MaterialHandle, material)

    // Gets/sets the render mode.
	ACESSOR(PolygonMode, PolygonMode::Enum, mode)

	// Gets/sets the primitive type.
	ACESSOR(PrimitiveType, PolygonType::Enum, type)

	// Pre-render callback.
	Delegate1<const RenderState&> onPreRender;

	// Post-render callback.
	Delegate1<const RenderState&> onPostRender;

protected:

	// Rendering stage.
	RenderLayer::Enum stage;

	// Rendering priority.
	int32 priority;

    // Primitive type.
    PolygonType::Enum type;

	// Polygon mode.
	PolygonMode::Enum mode;

	// Vertex buffer with all the vertices.
    VertexBufferPtr vb;

	// Index buffer with all the indices.
    IndexBufferPtr ib;
    
	// Material of this renderable.
	MaterialHandle material;
};

API_ENGINE Renderable* RenderableCreate(Allocator*);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Renderable );

//-----------------------------------//

} // end namespace