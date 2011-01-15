/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Component.h"
#include "math/Frustum.h"

namespace vapor {

//-----------------------------------//

class VAPOR_API Projector : public Component
{
	DECLARE_CLASS_()

public:

	Projector();

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Updates this component.
	void update( double delta );

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