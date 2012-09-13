/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Component.h"
#include "Engine/Scene/Transform.h"
#include "Engine/Scene/Camera.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Billboards come in different flavors:
 *
 * Screen aligned billboards are the simplest, they are always aligned
 * to the screen. Use these for text and GUI stuff.
 *
 * World aligned billboards will rotate in an arbitrary axis to face
 * the camera, thus they are useful for clouds and other distant objects.
 *
 * Axis aligned billboards can rotate only with respect to a given axis.
 * These are useful to represent cylindrical-symetric objects like trees
 * since you don't want them to rotate to face the camera when you fly
 * over them.
 */

API_SCENE REFLECT_DECLARE_ENUM(BillboardType)

enum struct BillboardType
{
	ScreenAligned,
	WorldAligned,
	AxisAligned,
};

//-----------------------------------//

/**
 * Billboards always face the direction of the camera.
 */

API_SCENE REFLECT_DECLARE_CLASS(Billboard)

class API_SCENE Billboard : public Component
{
	REFLECT_DECLARE_OBJECT(Billboard)

public:

	Billboard();
	Billboard( BillboardType type );

	// Updates the component.
	void update( float delta ) OVERRIDE;

	// Called just before the camera renders this component.
	void onPreRender( const Camera& camera ) OVERRIDE;

	// Creates the debug.renderable of the component.
	//RenderBatchPtr createDebugRenderable() const OVERRIDE;

protected:

	// Billboard type
	BillboardType billboardType;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Billboard );

//-----------------------------------//

NAMESPACE_ENGINE_END