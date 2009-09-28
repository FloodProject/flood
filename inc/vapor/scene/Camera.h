/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/math/Vector3.h"
#include "vapor/math/Matrix.h"
#include "vapor/math/Frustum.h"

#include "vapor/scene/Transform.h"

#include "vapor/render/Device.h"
#include "vapor/render/Target.h"
#include "vapor/render/Renderable.h"

namespace vapor {
	namespace scene {

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

class VAPOR_API Camera : public Transformable
{
public:

	Camera( render::Device* device, 
		Projection::Enum projection = Projection::Perspective );

	// Points the camera to a given point in space.
	void lookAt( const math::Vector3& pt );

	// Sets the projection type of the camera.
	void setProjection( Projection::Enum projection );

	// Sets the field-of-view of the camera.
	void setFOV( float fov );

	// Sets the far clipping plane of the camera.
	void setFar( float far );

	// Sets the near clipping plane of the camera.
	void setNear( float near );

	// Sets a new render target in the camera.
	void setRenderTarget( render::Target* target );

	// Gets the frustum associated with the camera.
	const math::Frustum& getFrustum( ) const;

	// Gets the current render target associated with the camera.
	render::Target* getRenderTarget() const;

	// Renders the (sub-)scene starting from the passed node to the current 
	// render target associated in the camera.
	void render( NodePtr node ) const;

	// Renders the entire scene starting from the root node to the current 
	// render target associated in the camera.
	void render() const;

	// Performs hierarchical frustum culling on the nodes in the scene 
	// starting from the given node. In other words, the camera will check 
	// all the nodes and return a list of those that are inside its frustum
	// for later rendering (and also their local to world matrices). 
	// The queue is passed as a reference to the cull method, which fills 
	// it with the data.
	void cull( render::RenderQueue& queue, NodePtr node ) const;

	// Updates this node.
	virtual void update( double delta );

	// Serializes this node.
	virtual const std::string save( int indent = 0 );
	
	// Gets the name of this node.
	virtual const std::string name() const;

protected:

	// Gets the aspect ratio of the target.
	float getAspectRatio() const;

	// Handles target resize (must update width, height).
	void handleTargetResize( const render::Settings& );

	// Sets up the projection matrices for OpenGL.
	void setupProjection();

	// Sets up the view matrices for OpenGL.
	void setupView();

	// Projection mode.
	Projection::Enum projection;
	
	// Field of view of this camera.
	float fov;

	// near and far are reserved keywords on MSVC.. so ghey! 
	float near_;
	float far_;
	
	// Why do we need this?
	math::Frustum* frustum;

	// Render target that we are rendering into.
	render::Target* target;

	// Used to pass a RenderQueue for rendering.
	render::Device* renderDevice;

	int width, height;

	//fd::delegate< const render::Settings& > targetResize;
};

//-----------------------------------//

typedef tr1::shared_ptr< Camera > CameraPtr;

//-----------------------------------//

} } // end namespaces
