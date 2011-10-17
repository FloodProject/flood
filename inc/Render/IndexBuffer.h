/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Buffer.h"

FWD_DECL_INTRUSIVE(GeometryBuffer)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class API_ENGINE IndexBuffer : public Buffer
{
public:

	IndexBuffer();

	// Updates the internal IBO with current values for indices.
	bool build(const GeometryBufferPtr&);

	// Binds/unbinds the index buffer from the OpenGL state.
	bool bind();
	bool unbind();

	bool isBuilt;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( IndexBuffer );

//-----------------------------------//

NAMESPACE_ENGINE_END
