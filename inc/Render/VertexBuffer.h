/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector.h"
#include "Math/Color.h"
#include "Render/Buffer.h"
#include "Geometry/GeometryBuffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

FWD_DECL_INTRUSIVE(GeometryBuffer)

/**
 * Represents a vertex buffer. One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

class API_RENDER VertexBuffer : public Buffer
{
public:

	VertexBuffer();
	~VertexBuffer();

	// Gets/sets the geometry buffer.
	ACESSOR(GeometryBuffer, GeometryBuffer*, gb)

	// Binds the vertex buffer.
	bool bind();
	
	// Unbinds the vertex buffer.
	bool unbind();

	// Binds the attributes pointers.
	void bindPointers();

	// Unbinds the attribute pointers.
	void unbindPointers();

	// Binds the attribute pointers.
	void bindGenericPointers();

	// Unbinds the attribute pointers.
	void unbindGenericPointers();
	
	// Returns if the this buffer is valid.
	bool isValid() const;

	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Updates the internal buffer with current attributes.
	bool build(const GeometryBufferPtr&);

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();

	// Keeps if the buffer has been built.
	bool built;

	// Keeps the geometry buffer associated with this buffer.
	GeometryBuffer* gb;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

NAMESPACE_ENGINE_END