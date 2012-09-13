/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Generates a procedural quad mesh.
 */

API_SCENE REFLECT_DECLARE_CLASS(Quad)

class API_SCENE Quad : public Geometry
{
	REFLECT_DECLARE_OBJECT(Quad)

public:

	Quad( float width = 1.0f, float height = 1.0f );

private:

	GeometryBufferPtr createQuad( float width, float height );
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Quad );

//-----------------------------------//

NAMESPACE_ENGINE_END
