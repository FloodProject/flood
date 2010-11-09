/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Color.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Matrix4x4.h"
#include "vapor/render/Target.h"

FWD_DECL_SHARED(Node)
FWD_DECL_SHARED(Camera)
FWD_DECL_SHARED_WEAK(Camera)

namespace vapor {

//-----------------------------------//

class RenderTarget;

//-----------------------------------//

/**
 * A view is a region in a render target that can be rendered into.
 * It has an associated camera that will render into the view.
 */

class VAPOR_API RenderView : private boost::noncopyable
{
public:

	RenderView();
	RenderView( const CameraPtr& );

	// Gets the camera of the view.
	GETTER(Camera, CameraPtr, weakCamera.lock())

	// Sets the camera of the view.
	SETTER(Camera, const CameraPtr&, weakCamera)

	// Gets/sets the render target of the view.
	ACESSOR(RenderTarget, RenderTarget*, target)

	// Gets the origin of the view.
	GETTER(Origin, Vector2i, Vector2i::Zero)

	// Gets the size of the view.
	GETTER(Size, Vector2i, target->getSettings().getSize())

	// Gets/sets the clear color of the view.
	ACESSOR(ClearColor, const Color&, clearColor)

	// Gets the aspect ratio of the view.
	float getAspectRatio() const;

	// Gets/sets the depth (Z-order) priority of the view.
	ACESSOR(DepthPriority, int, depthPriority)

	// Updates the view.
	void update();

	// Unprojects the point.
	Vector3 unprojectPoint( const Vector3& point,
		const Matrix4x4& projection, const Matrix4x3& view ) const;

	// Returns if a view has more priority than another.
	bool operator < (RenderView& v);

	// Handles render target resize.
	void handleRenderTargetResize();

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