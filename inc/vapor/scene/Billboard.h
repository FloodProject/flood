/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Camera.h"
#include "vapor/render/Renderable.h"

namespace vapor { namespace scene {

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

namespace BillboardType
{
	enum Enum
	{
		ScreenAligned,
		WorldAligned,
		AxisAligned,
	};
}

//-----------------------------------//

/**
 * Billboards always face the direction of the camera.
 */

class Billboard : public Component
{
public:

	Billboard( const CameraPtr& cam, BillboardType::Enum type =
		BillboardType::WorldAligned );
	virtual ~Billboard();

	void update( double delta );

	const std::string& getType() const;

	render::RenderablePtr getDebugRenderable() const;
  
private:

	// Billboard type
	BillboardType::Enum billboardType;

	TransformPtr transform;
	CameraPtr camera;
	render::RenderablePtr rend;

	// Component type
	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Billboard );

//-----------------------------------//

} } // end namespaces