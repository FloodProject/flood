/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Component.h"
#include "Math/Frustum.h"
#include "Math/Matrix4x3.h"

FWD_DECL_INTRUSIVE(Transform)
FWD_DECL_INTRUSIVE(Scene)

namespace vapor {

//-----------------------------------//

class  RenderView;
class  RenderDevice;
struct RenderBlock;

/**
 * Represents a view from a specific point in the world. Has an associated 
 * projection type, like ortographic or perspective and also holds a frustum 
 * that will be used for performing viewing frustum culling. Culling helps
 * speed up the rendering by cutting nodes that are outside the view range.
 */

REFLECT_DECLARE_CLASS(Camera)

class API_SCENE Camera : public Component
{
	REFLECT_DECLARE_OBJECT(Camera)
	DECLARE_UNCOPYABLE(Camera)
	
public:

	Camera();
	virtual ~Camera();

	// Renders the scene to the current render view.
	void render( const ScenePtr& scene );

	// Renders the block to the current render view.
	void render( RenderBlock& block, bool clearView = true );

	// Performs hierarchical frustum culling on the nodes in the scene.
	void cull( RenderBlock& queue, const EntityPtr& node );

	// Gets the look-at vector of the camera.
	GETTER(LookAtVector, const Vector3&, lookAtVector)

	// Gets the view matrix of the camera.
	GETTER(ViewMatrix, const Matrix4x3&, viewMatrix)
	
	// Gets the current view associated with the camera.
	GETTER(View, RenderView*, activeView)

	// Sets the current view associated with the camera.
	void setView(RenderView* view);

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }
	const Frustum& getFrustum() const { return frustum; }

	// Gets a ray given the screen coordinates of the mouse.
	Ray getRay( float x, float y, Vector3* outFar = nullptr ) const;

	// Gets a volume given the screen coordinates of the mouse.
	Frustum getVolume( float screenLeft, float screenRight, float screenTop, float screenBottom );

	// Updates this component.
	void update( float delta );

	// Updates the frustum.
	void updateFrustum();

protected:

	// Updates the view transform.
	void updateViewTransform();

	// Handles the transform notification.
	void onTransform();

	// Updates the debug renderable of the camera.
	void updateDebugRenderable() const;

	// Creates the debug renderable of the camera.
	RenderablePtr createDebugRenderable() const;

	// Camera frustum.
	Frustum frustum;

	// Frustum culling.
	bool frustumCulling;

	// Look-at vector.
	Vector3 lookAtVector;

	// View matrix.
	Matrix4x3 viewMatrix;

	// Pointer to the camera node transform.
	TransformPtr transform;

	// Last view active the camera.
	RenderView* activeView;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Camera );

//-----------------------------------//

} // end namespace
