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
#include "vapor/render/Viewport.h"
#include "vapor/scene/Transform.h"

namespace vapor {

//-----------------------------------//

/**
 * Types of camera projection.
 */

namespace Projection 
{
    enum Enum
    {
        Orthographic,
        Perspective
    };
}

//-----------------------------------//

/**
 * Represents a view from a specific point in the world. Has an associated 
 * projection type, like ortographic or perspective and also holds a frustum 
 * that will be used for performing viewing frustum culling. Culling helps
 * speed up the rendering by cutting nodes that are outside of the view range.
 */

class VAPOR_API Camera : public Component
{
public:

	Camera( RenderDevice*, Projection::Enum = Projection::Perspective );
	Camera( const Camera& rhs );
	~Camera();

	// Renders the (sub-)scene starting from the passed node to the current 
	// render target associated in the camera.
	void render( const NodePtr& node, bool clearView = true ) const;

	// Renders the entire scene starting from the root node to the current 
	// render target associated in the camera.
	void render() const;

	// Performs hierarchical frustum culling on the nodes in the scene 
	// starting from the given node. In other words, the camera will check 
	// all the nodes and return a list of those that are inside its frustum
	// for later rendering (and also their local to world matrices). 
	// The queue is passed as a reference to the cull method, which fills 
	// it with the data.
	void cull( RenderBlock& queue, const NodePtr& node ) const;

	// Unprojects the vector.
	Vector3 Unproject( Vector3 vector ) const;

	// Gets a ray given the screen coordinates of the mouse.
	Ray getRay( float scrx, float scry, Vector3* outFar = nullptr ) const;

	// Updates this node.
	virtual void update( double delta );

	// Gets/sets the projection type of the camera.
	ACESSOR(Projection, Projection::Enum, projection)

	// Gets/sets the field-of-view of the camera.
	ACESSOR(FOV, float, fov);

	// Gets/sets the far clipping plane of the camera.
	ACESSOR(Far, float, far_);

	// Gets/sets the near clipping plane of the camera.
	ACESSOR(Near, float, near_);

	// Gets the look-at vector of the camera.
	GETTER(LookAtVector, const Vector3&, lookAtVector)

	// Gets the projection matrix of the camera.
	GETTER(ProjectionMatrix, const Matrix4x4&, projectionMatrix)

	// Gets the view matrix of the camera.
	GETTER(ViewMatrix, const Matrix4x3&, viewMatrix)
	
	// Gets/sets the current viewport associated with the camera.
	GETTER(Viewport, ViewportPtr, viewport)
	DECLARE_SETTER(Viewport, ViewportPtr)

	// Gets the frustum associated with the camera.
	//GETTER(Frustum, const Frustum&, frustum)
	
	// Gets the type of this node.
	GETTER(Type, const std::string&, type)

	DECLARE_SERIALIZABLE();

protected:

	// Sets up the projection matrix.
	void setupProjection( const Vector2i& size );

	// Sets up the view matrix.
	void setupView();

	// Handles the transform notification.
	void onTransform();

	// Look-at vector.
	Vector3 lookAtVector;

	// View matrix.
	Matrix4x3 viewMatrix;

	// Projection matrix.
	Matrix4x4 projectionMatrix;

	// Projection mode.
	Projection::Enum projection;
	
	// Field of view of this camera.
	float fov;

	// Near and far clipping planes values.
	// Note: near and far are reserved keywords on MSVC. 
	float near_;
	float far_;

	// Last viewport the camera rendered into.
	ViewportPtr viewport;
	Vector2i viewSize;

	// Pointer to the camera's node transform.
	TransformPtr transform;

	// Used to pass a RenderQueue for rendering.
	RenderDevicePtr renderDevice;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Camera );
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Camera );

//-----------------------------------//

} // end namespace
