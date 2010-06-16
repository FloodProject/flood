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

FWD_DECL_TYPEDEF_PTR(RenderTarget)
FWD_DECL_TYPEDEF_SHARED(Node)
FWD_DECL_TYPEDEF_SHARED(Camera)
FWD_DECL_TYPEDEF_SHARED_WEAK(Camera)

namespace vapor {

//-----------------------------------//

/**
 * A viewport is a region in a render target that can be rendered into.
 * It has an associated camera that will update the viewport is updated.
 */

class VAPOR_API Viewport : private boost::noncopyable
{
public:

	Viewport( CameraPtr, RenderTargetPtr );

	// Gets the camera of the viewport.
	GETTER(Camera, CameraPtr, weakCamera.lock())

	// Gets the render target of the viewport.
	GETTER(RenderTarget, RenderTargetPtr, target)

	// Gets the size of the viewport.
	DECLARE_GETTER(Size, Vector2i)
	SETTER(Size, Vector2i, size)

	// Gets/sets the clear color of the viewport.
	ACESSOR(ClearColor, const Color&, clearColor)

	// Gets the aspect ratio of the viewport.
	float getAspectRatio() const;

	// Gets/sets the Z-order priority of the viewport.
	ACESSOR(Priority, int, zPriority)

	// Updates the viewport.
	void update();

	// Returns if a viewport has more priority than another.
	bool operator < (Viewport& v);

private:

	// Sets a new render target in the camera.
	void setRenderTarget( RenderTargetPtr target );

	// Camera that will render into this viewport.
	CameraWeakPtr weakCamera;

	// Render target that we are rendering into.
	RenderTargetPtr target;
	
	// Dimensions of render target.
	Vector2i size;

	// Background color.
	Color clearColor;

	// Z-order priority.
	int zPriority;
};

TYPEDEF_PTR(Viewport)

//-----------------------------------//

} // end namespace
