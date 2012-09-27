/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * This enum provides all the render commands.
 */

enum struct RenderCommandType : uint8
{
	RenderState,
	SetViewport,
	ClearViewport,
	CreateTexture,
	CreateVertexBuffer,
	BindVertexBuffer,
	BindIndexBuffer,
	SetVertexBufferData,
	DrawCall
};

/**
 * Render commands abstract the actual rendering API-specific commands.
 * They are also designed to be usable in a efficient and data-driven
 * way.
 */

struct API_GRAPHICS RenderCommand
{
	RenderCommand(RenderCommandType);

	RenderCommandType type;
};

//-----------------------------------//

/**
 * Type of primitive of the draw call.
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

struct API_GRAPHICS ClearViewportCommand : public RenderCommand
{
	ClearViewportCommand();

	ColorP color;
};

struct API_GRAPHICS BindVertexBufferCommand : public RenderCommand
{
	BindVertexBufferCommand();

	VertexBufferId vertexBuffer;
};

struct API_GRAPHICS BindIndexBufferCommand : public RenderCommand
{
	BindIndexBufferCommand();

	IndexBufferId indexBuffer;
};

/** The range of indices in the draw call */

struct API_GRAPHICS RenderBatchRange
{
	RenderBatchRange();

	RenderIndexOffset start;
	RenderIndexOffset end;
};

struct API_GRAPHICS DrawCallCommand : public RenderCommand
{
	DrawCallCommand();

	PrimitiveType primitive;
	RenderBatchRange range;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END