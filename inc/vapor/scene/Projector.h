/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Transform.h"
#include "vapor/render/RenderQueue.h"
#include "vapor/math/BoundingBox.h"
#include "vapor/math/Frustum.h"

namespace vapor {

//-----------------------------------//

class VAPOR_API Projector : public Component
{
	DECLARE_CLASS_()

public:

	Projector();

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

protected:

	// Projector frustum.
	Frustum frustum;
};

//-----------------------------------//

TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Projector );
TYPEDEF_SHARED_POINTER_FROM_TYPE( Projector );

//-----------------------------------//

} // end namespace