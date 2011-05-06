/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Component.h"
#include "Math/Frustum.h"

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(Projector)

class VAPOR_API Projector : public Component
{
	REFLECT_DECLARE_OBJECT(Projector)

public:

	Projector();

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Updates this component.
	void update( float delta );

	// Updates the debug renderable of the camera.
	void updateDebugRenderable() const;

	// Creates the debug renderable of the camera.
	RenderablePtr createDebugRenderable() const;

protected:

	// Projector frustum.
	Frustum frustum;
};

TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Projector );
TYPEDEF_SHARED_POINTER_FROM_TYPE( Projector );

//-----------------------------------//

} // end namespace