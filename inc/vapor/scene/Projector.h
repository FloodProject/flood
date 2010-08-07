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
public:

	Projector();

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Returns the name of this component.
	GETTER(Type, const std::string&, type)

	DECLARE_SERIALIZABLE();

protected:

	// Projector frustum.
	Frustum frustum;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Projector );
TYPEDEF_SHARED_POINTER_FROM_TYPE( Projector );

//-----------------------------------//

} // end namespace