/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Component.h"
#include "vapor/math/Frustum.h"

FWD_DECL_SHARED(Transform)

namespace vapor {

//-----------------------------------//

class View;
class RenderDevice;
struct RenderBlock;

//-----------------------------------//

/**
 * Represents a view from a specific point in the world. Has an associated 
 * projection type, like ortographic or perspective and also holds a frustum 
 * that will be used for performing viewing frustum culling. Culling helps
 * speed up the rendering by cutting nodes that are outside the view range.
 */

class VAPOR_API Camera : public Component
{
	DECLARE_CLASS_()
	
public:

	Camera();
	Camera( RenderDevice* );
	Camera( const Camera& rhs );
	~Camera();

	// Renders the scene to the current render view.
	void render();

	// Renders the scene to the current render view.
	void render( const NodePtr& node, bool clearView = true );

	// Performs hierarchical frustum culling on the nodes in the scene.
	void cull( RenderBlock& queue, const NodePtr& node ) const;

	// Gets a ray given the screen coordinates of the mouse.
	Ray getRay( float scrx, float scry, Vector3* outFar = nullptr ) const;

	// Updates this component.
	virtual void update( double delta );

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Gets the look-at vector of the camera.
	GETTER(LookAtVector, const Vector3&, lookAtVector)

	// Gets the view matrix of the camera.
	GETTER(ViewMatrix, const Matrix4x3&, viewMatrix)
	
	// Gets the current view associated with the camera.
	GETTER(View, View*, activeView)

	// Sets the current view associated with the camera.
	DECLARE_SETTER(View, View*)

protected:

	// Initializes the camera.
	void init();

	// Sets up the view transform.
	void updateViewTransform();

	// Handles the transform notification.
	void onTransform();

	// Updates the debug renderable of the camera.
	void updateDebugRenderable() const;

	// Creates the debug renderable of the camera.
	virtual RenderablePtr createDebugRenderable() const;

	// Camera frustum.
	Frustum frustum;

	// Look-at vector.
	Vector3 lookAtVector;

	// View matrix.
	Matrix4x3 viewMatrix;

	// Last view active the camera.
	View* activeView;

	// Pointer to the camera node transform.
	TransformPtr transform;

	// Rendering device.
	RenderDevice* renderDevice;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Camera );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Camera );

//-----------------------------------//

} // end namespace
