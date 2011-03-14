/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Component.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Render/Renderable.h"

namespace vapor {

//-----------------------------------//

/**
 * Billboards come in different flavors. Screen aligned billboards are
 * the simplest, they are always aligned to the screen. Use these for
 * text and GUI stuff. World aligned billboards will rotate in an
 * arbitrary axis to face the camera, thus they are useful for clouds
 * and other distant objects. Axis aligned billboards can rotate only
 * with respect to a given axis. These are most useful to represent
 * trees. You don't want trees to rotate to face the camera when you
 * fly over them, since that would break the illusion.
 */

struct BillboardType
{
	DECLARE_ENUM()

	enum Enum
	{
		ScreenAligned,
		WorldAligned,
		AxisAligned,
	};
};

//-----------------------------------//

/**
 * Billboards always face the direction of the camera.
 */

class VAPOR_API Billboard : public Component
{
	DECLARE_CLASS_()

public:

	Billboard();
	Billboard( BillboardType::Enum type );

	// Updates the component.
	void update( float delta );

	// Called just before the camera renders this component.
	void onPreRender( const Camera& camera );

	// Creates the debug renderable of the component.
	RenderablePtr createDebugRenderable() const;
  
protected:

	// Billboard type
	BillboardType::Enum billboardType;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Billboard );

//-----------------------------------//

} // end namespace