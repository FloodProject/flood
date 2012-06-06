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
#include "Graphics/Buffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

FWD_DECL_INTRUSIVE(GeometryBuffer)

/**
 * Represents a vertex buffer. One limitation here is that all data is 
 * tied to the vertex so if you want a normal per primitive and not per 
 * vertex you will have to duplicate that normal for each vertex for now.
 */

typedef BufferId VertexBufferId;

class API_RENDER VertexBuffer : public Buffer
{
public:

	VertexBuffer();
	~VertexBuffer();
	
	// Returns true if the vertex buffer is built, false otherwhise.
	bool isBuilt() const;

	// Forces a rebuild of the vertex buffer the next update.
	void forceRebuild();

	// Keeps if the buffer has been built.
	bool built;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( VertexBuffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END