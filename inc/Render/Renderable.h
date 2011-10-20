/************************************************************************
*
*  vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Delegate.h"
#include "Resources/Material.h"
#include "Geometry/GeometryBuffer.h"

#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/UniformBuffer.h"

FWD_DECL_INTRUSIVE(Material)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Type of primitive of the renderable.
 */

namespace PolygonType
{
    enum Enum
    {
        Points,
        Lines,
        LineLoop,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan,
        Quads,
        QuadStrip,
        Polygon
    };
}

//-----------------------------------//

/**
 * Type of rendering mode of the renderable.
 */

#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02

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

/**
 * Represents a renderable object, that contains the details the
 * rendering device needs to render the object. At the least, it
 * should have a material and a vertex buffer.
 */

class RenderView;
struct RenderState;

class API_RENDER Renderable : public ReferenceCounted
{
public:

	Renderable();

    // Gets/sets the render stage.
	ACESSOR(RenderLayer, RenderLayer::Enum, stage)
   
    // Gets/sets the render priority.
	ACESSOR(RenderPriority, int32, priority)

    // Gets/sets the geometry buffer.
	ACESSOR(GeometryBuffer, const GeometryBufferPtr&, gb)
    
    // Gets/sets the index buffer.
	ACESSOR(UniformBuffer, const UniformBufferPtr&, ub)

    // Gets/sets the material.
    ACESSOR(Material, const MaterialHandle&, material)

    // Gets/sets the render mode.
	ACESSOR(PolygonMode, PolygonMode::Enum, mode)

	// Gets/sets the primitive type.
	ACESSOR(PrimitiveType, PolygonType::Enum, type)

	// Pre-render callback.
	Delegate2<RenderView*, const RenderState&> onPreRender;

	// Post-render callback.
	Delegate2<RenderView*, const RenderState&> onPostRender;

protected:

	// Rendering stage.
	RenderLayer::Enum stage;

	// Rendering priority.
	int32 priority;

    // Primitive type.
    PolygonType::Enum type;

	// Polygon mode.
	PolygonMode::Enum mode;

	// Geometry buffer with the geometry data.
	GeometryBufferPtr gb;

	// Uniform buffer with shader constants.
	UniformBufferPtr ub;
    
	// Material of this renderable.
	MaterialHandle material;
};

API_ENGINE Renderable* RenderableCreate(Allocator*);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Renderable );
typedef std::vector<RenderablePtr> RenderablesVector;

//-----------------------------------//

NAMESPACE_ENGINE_END