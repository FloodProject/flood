/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "math/Color.h"
#include "math/Vector2.h"
#include "math/Matrix4x3.h"
#include "math/Matrix4x4.h"
#include "render/Target.h"

FWD_DECL_SHARED(Entity)
FWD_DECL_SHARED(Camera)
FWD_DECL_SHARED_WEAK(Camera)

namespace vapor {

//-----------------------------------//

class RenderTarget;

/**
 * A view is a region in a render target that can be rendered into.
 * It has an associated camera that will render into the view.
 */

class VAPOR_API RenderView
{
	DECLARE_UNCOPYABLE(RenderView)

public:

	RenderView();
	RenderView( const CameraPtr& );

	// Gets the camera of the view.
	GETTER(Camera, CameraPtr, weakCamera.lock())

	// Sets the camera of the view.
	void setCamera( const CameraPtr& camera );

	// Gets/sets the render target of the view.
	ACESSOR(RenderTarget, RenderTarget*, target)

	// Gets the origin of the view.
	GETTER(Origin, Vector2i, Vector2i::Zero)

	// Gets the size of the view.
	GETTER(Size, Vector2i, target->getSettings().getSize())

	// Gets/sets the clear color of the view.
	ACESSOR(ClearColor, const Color&, clearColor)

	// Gets/sets the depth (Z-order) priority of the view.
	ACESSOR(DepthPriority, int, depthPriority)

	// Gets the aspect ratio of the view.
	float getAspectRatio() const;

	// Updates the view.
	void update();

	// Unprojects the point.
	Vector3 unprojectPoint( const Vector3& pt, const Camera* camera ) const;

	// Returns if a view has more priority than another.
	bool operator < (RenderView& view);

	// Handles render target resize.
	void handleRenderTargetResize();

	// Event is sent when camera changes.
	Event1<const CameraPtr&> onCameraChanged; 

protected:

	// Camera that will render into this view.
	CameraWeakPtr weakCamera;

	// Render target.
	RenderTarget* target;

	// Background color.
	Color clearColor;

	// Depth priority.
	int depthPriority;
};

//-----------------------------------//

} // end namespace