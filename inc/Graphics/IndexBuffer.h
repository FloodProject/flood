/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/Buffer.h"

FWD_DECL_INTRUSIVE(GeometryBuffer)

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

typedef BufferId IndexBufferId;

class API_GRAPHICS IndexBuffer : public Buffer
{
public:

	IndexBuffer();
	bool isBuilt;
};

typedef uint16 RenderIndexOffset;

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( IndexBuffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END
