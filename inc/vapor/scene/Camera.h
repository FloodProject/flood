/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/Matrix.h"
#include "vapor/math/Frustum.h"

#include "vapor/scene/Transform.h"

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

// This is a queue of objects that are usually returned by performing a culling
// operation on the camera.
typedef std::list< 
	std::pair< shared_ptr< render::Renderable >, math::Matrix4*> > RenderQueue;

//-----------------------------------//

/**
 * Represents a view from a specific point in the world. Has an associated 
 * projection type, like ortographic or perspective and also holds a frustum 
 * that will be used for performing viewing frustum culling. Culling helps
 * speed up the rendering by cutting nodes that are outside of the view range.
 */

class Camera : public Transformable
{
public:

	Camera( render::Device* device );

	// Sets the projection type of the camera.
	void setProjection( Projection::Enum projection );

	// Sets the field-of-view of the camera.
	void setFOV( float fov );

	// Sets the far clipping plane of the camera.
	void setFar( float far );

	// Sets the near clipping plane of the camera.
	void setNear( float near );

	// Sets a new render target in the camera.
	void setRenderTarget( shared_ptr< render::Target > target );

	// Gets the frustum associated with the camera.
	const math::Frustum& getFrustum( );

	// Gets the current render target associated with the camera.
	shared_ptr< render::Target > getRenderTarget() const;

	// Renders the (sub-)scene starting from the passed node to the current 
	// render target associated in the camera.
	void render( shared_ptr< Node > node ) const;
	//{
	//  RenderQueue queue;
	//  
	//  cull( queue, node );
	//  
	//  renderTarget->makeCurrent();
	//  
	//  renderDevice->render( queue, absoluteLocalToWorld.inverse() );
	//}

	// Renders the entire scene starting from the root node to the current 
	// render target associated in the camera.
	void render() const;
	//{
	//  shared_ptr< Node > parent( getParent() );
	//  
	//  while ( parent->getParent() )
	//	  parent = parent->getParent();
	//      
	//  render( parent );
	//}

	// Performs hierarchical frustum culling on the nodes in the scene starting from the given node.
	// In other words, the camera will check all the nodes and return a list of those that are
	// inside its frustum for later rendering (and also their local to world matrices).
	// The queue is passed as a reference to the cull method, which fills it with the data.
	void cull( RenderQueue& queue, shared_ptr< Node > root ) const;

private:

	Projection::Enum projection;
	
	float FOV;

	// near and far are reserved keywords on MSVC.. so ghey! 
	float near_;
	float far_;
	
	math::Frustum* frustum;

	shared_ptr< render::Target > target;
	shared_ptr< render::Device > renderDevice;
};

//-----------------------------------//

} } // end namespaces
