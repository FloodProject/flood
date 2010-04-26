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

FWD_DECL_NS_TYPEDEF_PTR(render, RenderTarget)

FWD_DECL_NS_TYPEDEF_SHARED(scene, Node)
FWD_DECL_NS_TYPEDEF_SHARED(scene, Camera)
FWD_DECL_NS_TYPEDEF_SHARED_WEAK(scene, Camera)

namespace vapor { namespace render {

//-----------------------------------//

/**
 * A viewport is a region in a render target that can be rendered into.
 * It has an associated camera that will update the viewport is updated.
 */

class VAPOR_API Viewport : private boost::noncopyable
{
public:

	Viewport( scene::CameraPtr, RenderTargetPtr );

	// Gets the camera of the viewport.
	IMPLEMENT_GETTER(Camera, scene::CameraPtr, weakCamera.lock())

	// Gets the render target of the viewport.
	IMPLEMENT_GETTER(RenderTarget, RenderTargetPtr, target)

	// Gets the size of the viewport.
	DECLARE_GETTER(Size, math::Vector2i)
	IMPLEMENT_SETTER(Size, math::Vector2i, size)

	// Gets/sets the clear color of the viewport.
	IMPLEMENT_ACESSOR(ClearColor, const math::Color&, clearColor)

	// Gets the aspect ratio of the viewport.
	float getAspectRatio() const;

	// Gets/sets the Z-order priority of the viewport.
	IMPLEMENT_ACESSOR(Priority, int, zPriority)

	// Updates the viewport.
	void update();

	// Returns if a viewport has more priority than another.
	bool operator < (Viewport& v);

private:

	// Sets a new render target in the camera.
	void setRenderTarget( RenderTargetPtr target );

	// Camera that will render into this viewport.
	scene::CameraWeakPtr weakCamera;

	// Render target that we are rendering into.
	RenderTargetPtr target;
	
	// Dimensions of render target.
	math::Vector2i size;

	// Background color.
	math::Color clearColor;

	// Z-order priority.
	int zPriority;
};

TYPEDEF_PTR(Viewport)

//-----------------------------------//

} } // end namespaces
