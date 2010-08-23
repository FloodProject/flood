/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Matrix4x4.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/Frustum.h"
#include "vapor/render/Device.h"
#include "vapor/render/View.h"
#include "vapor/scene/Transform.h"

namespace vapor {

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

	// Gets the camera frustum.
	Frustum& getFrustum() { return frustum; }

	// Renders the (sub-)scene starting from the passed node to the current 
	// render target associated in the camera.
	void render( const NodePtr& node, bool clearView = true );

	// Renders the entire scene starting from the root node to the current 
	// render target associated in the camera.
	void render();

	// Performs hierarchical frustum culling on the nodes in the scene 
	// starting from the given node. In other words, the camera will check 
	// all the nodes and return a list of those that are inside its frustum
	// for later rendering (and also their local to world matrices). 
	// The queue is passed as a reference to the cull method, which fills 
	// it with the data.
	void cull( RenderBlock& queue, const NodePtr& node ) const;

	// Gets a ray given the screen coordinates of the mouse.
	Ray getRay( float scrx, float scry, Vector3* outFar = nullptr ) const;

	// Updates this component.
	virtual void update( double delta );

	// Gets the look-at vector of the camera.
	GETTER(LookAtVector, const Vector3&, lookAtVector)

	// Gets the view matrix of the camera.
	GETTER(ViewMatrix, const Matrix4x3&, viewMatrix)
	
	// Gets/sets the current view associated with the camera.
	GETTER(View, View*, activeView)
	DECLARE_SETTER(View, View*)

protected:

	// Sets up the view transform.
	void updateViewTransform();

	// Handles the transform notification.
	void onTransform();

	// Camera frustum.
	Frustum frustum;

	// Look-at vector.
	Vector3 lookAtVector;

	// View matrix.
	Matrix4x3 viewMatrix;

	// Last view the camera rendered into.
	View* activeView;

	// Pointer to the camera's node transform.
	TransformPtr transform;

	// Used to pass a RenderQueue for rendering.
	RenderDevice* renderDevice;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Camera );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Camera );

//-----------------------------------//

} // end namespace
