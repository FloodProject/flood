/************************************************************************
*
*  Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Delegate.h"
#include "Graphics/GeometryBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/Resources/Material.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Type of primitive of the RenderBatch.
 */

enum struct PrimitiveType : uint8
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

//-----------------------------------//

/**
 * Type of rendering mode of the RenderBatch.
 */

enum struct PrimitiveRasterMode : uint8
{
	Solid,
	Wireframe,
	Point
};

//-----------------------------------//

/**
 * Use these different kinds of render groups to signal to the renderer
 * how you want your RenderBatchs to be sorted by the render device.
 * Lower numbers render before higher numbers.
 */

enum struct RenderLayer : uint8
{
	Normal = 0,
	Transparency = 5,
	PostTransparency = 7,
	Overlays = 10
};

//-----------------------------------//

/** The range of indices in the render batch */

struct API_GRAPHICS RenderBatchRange
{
	RenderBatchRange();

	RenderIndexOffset start;
	RenderIndexOffset end;
};

/**
 * Represents a batch operation in the render proccess and contains the
 * data the render device needs to issue the draw calls for the object.
 */

class RenderView;
class RenderState;

class API_GRAPHICS RenderBatch : public ReferenceCounted
{
public:

	RenderBatch();
	~RenderBatch();

	// Gets/sets the render stage.
	ACESSOR(RenderLayer, RenderLayer, stage)

	// Gets/sets the render priority.
	ACESSOR(RenderPriority, int32, priority)

	// Gets/sets the geometry buffer.
	ACESSOR(GeometryBuffer, const GeometryBufferPtr&, gb)

	// Gets/sets the index buffer.
	ACESSOR(UniformBuffer, const UniformBufferPtr&, ub)

	// Gets/sets the material.
	ACESSOR(Material, const MaterialHandle&, material)

	// Gets/sets the render mode.
	ACESSOR(PrimitiveRasterMode, PrimitiveRasterMode, mode)

	// Gets/sets the primitive type.
	ACESSOR(PrimitiveType, PrimitiveType, type)

	// Pre-render callback.
	Delegate2<RenderView*, const RenderState&> onPreRender;

	// Post-render callback.
	Delegate2<RenderView*, const RenderState&> onPostRender;

public:

	// Index range.
	RenderBatchRange range;

	// Rendering stage.
	RenderLayer stage;

	// Rendering priority.
	int32 priority;

	// Primitive type.
	PrimitiveType type;

	// Polygon mode.
	PrimitiveRasterMode mode;

	// Geometry buffer with the geometry data.
	GeometryBufferPtr gb;

	// Uniform buffer with shader constants.
	UniformBufferPtr ub;

	// Material of this batch.
	MaterialHandle material;
};

typedef RenderBatch Renderable;

API_GRAPHICS RenderBatch* RenderBatchCreate(Allocator*);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( RenderBatch );
typedef std::vector<RenderBatchPtr> RenderBatchsVector;

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Renderable );
typedef std::vector<RenderablePtr> RenderablesVector;

//-----------------------------------//

NAMESPACE_GRAPHICS_END