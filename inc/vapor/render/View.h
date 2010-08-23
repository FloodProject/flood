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

class VAPOR_API View : private boost::noncopyable
{
public:

	View( CameraPtr, RenderTarget* );

	// Gets the camera of the view.
	GETTER(Camera, CameraPtr, weakCamera.lock())

	// Gets the render target of the view.
	GETTER(RenderTarget, RenderTarget*, target)

	// Gets the origin of the view.
	GETTER(Origin, Vector2i, Vector2i::Zero)

	// Gets the size of the view.
	DECLARE_GETTER(Size, Vector2i)

	// Gets/sets the clear color of the view.
	ACESSOR(ClearColor, const Color&, clearColor)

	// Gets the aspect ratio of the view.
	float getAspectRatio() const;

	// Gets/sets the Z-order priority of the view.
	ACESSOR(Priority, int, zPriority)

	// Updates the view.
	void update();

	// Unprojects the vector.
	Vector3 unprojectPoint( const Vector3& vector,
		const Matrix4x4& projection, const Matrix4x3& view ) const;

	// Returns if a view has more priority than another.
	bool operator < (View& v);

private:

	// Sets a new render target in the camera.
	void setRenderTarget( RenderTarget* target );

	// Camera that will render into this view.
	CameraWeakPtr weakCamera;

	// Render target that we are rendering into.
	RenderTarget* target;
	
	// Background color.
	Color clearColor;

	// Z-order priority.
	int zPriority;
};

//-----------------------------------//

} // end namespace