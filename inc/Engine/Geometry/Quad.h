/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Generates a procedural quad mesh.
 */

 REFLECT_DECLARE_CLASS(Quad)

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

NAMESPACE_GRAPHICS_END