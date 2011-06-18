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

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Billboards come in different flavors. Screen aligned billboards are
 * the simplest, they are always aligned to the screen. Use these for
 * text and GUI stuff. World aligned billboards will rotate in an
 * arbitrary axis to face the camera, thus they are useful for clouds
 * and other distant objects. Axis aligned billboards can rotate only
 * with respect to a given axis. These are most useful to represent
 * cylindrical-symetric objects like trees since you don't want them to
 * to rotate to face the camera when you fly over them.
 */

struct API_SCENE BillboardType
{
	REFLECT_DECLARE_ENUM(BillboardType)

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

REFLECT_DECLARE_CLASS(Billboard)

class API_SCENE Billboard : public Component
{
	REFLECT_DECLARE_OBJECT(Billboard)

public:

	Billboard();
	Billboard( BillboardType::Enum type );

	// Updates the component.
	void update( float delta ) OVERRIDE;

	// Called just before the camera renders this component.
	void onPreRender( const Camera& camera ) OVERRIDE;

	// Creates the debug renderable of the component.
	RenderablePtr createDebugRenderable() const OVERRIDE;
  
protected:

	// Billboard type
	BillboardType::Enum billboardType;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Billboard );

//-----------------------------------//

NAMESPACE_END