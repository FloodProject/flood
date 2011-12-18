/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Buffer.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Geometry/GeometryBuffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

struct BufferEntry
{
	BufferEntry();

	VertexBufferPtr vb;
	IndexBufferPtr ib;
};

typedef std::map<const GeometryBuffer*, BufferEntry> BuffersMap;
typedef std::pair<const String&, BufferPtr> BuffersPair;

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

protected:

	BuffersMap buffers;
};

//-----------------------------------//

NAMESPACE_ENGINE_END