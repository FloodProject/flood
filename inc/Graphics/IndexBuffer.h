/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/Buffer.h"

FWD_DECL_INTRUSIVE(GeometryBuffer)

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

typedef BufferId IndexBufferId;

struct API_GRAPHICS IndexBuffer : public Buffer
{
	IndexBuffer();
	bool isBuilt;
};

typedef uint16 RenderIndexOffset;

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( IndexBuffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END
