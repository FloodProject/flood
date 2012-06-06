/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/Buffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Geometry/GeometryBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

struct BufferEntry
{
	BufferEntry();

	VertexBufferPtr vb;
	IndexBufferPtr ib;
};

typedef std::map<const GeometryBuffer*, BufferEntry> BuffersMap;
typedef std::pair<const String&, BufferPtr> BuffersPair;

class RenderBackend;

/**
 * Manages a set of buffers.
 */

class API_RENDER BufferManager
{
	DECLARE_UNCOPYABLE(BufferManager)

public:

	BufferManager();
	~BufferManager();

	// Creates and gets a buffer.
	BufferEntry* getBuffer(const GeometryBuffer*);

	// Gets a vertex buffer with the the geometry given.
	VertexBufferPtr getVertexBuffer(const GeometryBuffer*);

	// Gets an index buffer with the geometry given.
	IndexBufferPtr getIndexBuffer(const GeometryBuffer*);

	// Sets the rendering backend.
	SETTER(RenderBackend, RenderBackend*, backend)

protected:

	RenderBackend* backend;
	BuffersMap buffers;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END