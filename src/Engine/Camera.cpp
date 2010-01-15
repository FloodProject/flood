/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Camera.h"
#include "vapor/scene/Group.h"
#include "vapor/scene/Geometry.h"

namespace vapor { namespace scene {

using namespace vapor::math;
using namespace vapor::render;

//-----------------------------------//

const std::string& Camera::type = "Camera";

//-----------------------------------//

Camera::Camera( render::Device* device, Projection::Enum projection )
	: renderDevice( device ), projection( projection ),
		target( nullptr ), fov(45.0f), near_( 5.0f ), far_( 10000.0f ),
		lookAtVector( Vector3( 0.0f, 0.0f, 0.0f ) ), 
		forwardVector( Vector3::UnitZ ), upVector( Vector3::UnitY )
{
	setRenderTarget( device->getRenderTarget() );
}

//-----------------------------------//

void Camera::setProjection( Projection::Enum projection )
{
	this->projection = projection;
}

//-----------------------------------//

float Camera::getFOV() const
{
	return fov;
}

//-----------------------------------//

const math::Matrix4x4& Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

//-----------------------------------//

const math::Matrix4x3& Camera::getViewMatrix() const
{
	return viewMatrix;
}

//-----------------------------------//

render::RenderTarget* Camera::getRenderTarget() const
{
	return target;
}

//-----------------------------------//

void Camera::setFOV( float fov )
{
	this->fov = fov;
}

//-----------------------------------//

void Camera::setNear( float near_ )
{
	this->near_ = near_;
}

//-----------------------------------//

float Camera::getNear() const
{
	return near_;
}


//-----------------------------------//

void Camera::setFar( float far_ )
{
	this->far_ = far_;
}

//-----------------------------------//

float Camera::getFar() const
{
	return far_;
}

//-----------------------------------//

void Camera::setRenderTarget( RenderTarget* newTarget )
{
	if( !newTarget ) return;
	
	// TODO: remove only this from the delegate
	//target->onTargetResize.clear();

	target = newTarget;
	target->onTargetResize += fd::bind( &Camera::handleTargetResize, this );

	handleTargetResize( target->getSettings() );
}

//-----------------------------------//

void Camera::handleTargetResize( const render::Settings& evt )
{
	width = evt.getWidth();
	height = evt.getHeight();

	glViewport( 0, 0, width, height );
}

//-----------------------------------//

void Camera::update( float UNUSED(delta) )
{
	if( !transform )
	{
		assert( getNode() != nullptr );
		assert( getNode()->getTransform() != nullptr );
		transform = getNode()->getTransform();
	}

	// We need to update both projection and view matrices.
	// TODO: optimize this so the projection is only updated when needed
	setupProjection();
	setupView();
}

//-----------------------------------//

void Camera::setupProjection()
{
	if( projection == Projection::Perspective )
	{
		projectionMatrix = Matrix4x4::createPerspectiveProjection( 
			fov, getAspectRatio(), near_, far_ );
	}
	else
	{
		projectionMatrix = Matrix4x4::createOrthographicProjection( 
			0.0f, width, 0.0f, height, near_, far_ );
	}

	// TODO: remove this, unit test projection methods
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//glOrtho( -target->getSettings().getWidth()/16, target->getSettings().getWidth()/16, 
	//	-target->getSettings().getHeight()/16, target->getSettings().getHeight()/16, 1.0f, 100.0f );
	////glOrtho( 0.0, target->getSettings().getWidth(), 0.0, target->getSettings().getHeight(), near_, far_ );
	//gluPerspective( fov, getAspectRatio(), near_, far_ );
	//GLfloat test[16];
	//glGetFloatv( GL_PROJECTION_MATRIX, &test[0] );
}

//-----------------------------------//

void Camera::setupView()
{
	// TODO: check if this is working with multiple nodes
	viewMatrix = transform->getAbsoluteTransform();
}

//-----------------------------------//

float Camera::getAspectRatio() const
{
	if( height == 0 ) return 0.0f;
	return static_cast<float>(width) / height ;
}

//-----------------------------------//

//const math::Frustum& Camera::getFrustum( ) const
//{
//	return new math::Frustum( );
//}

//-----------------------------------//

void Camera::render( NodePtr node ) const
{
	// This will contain all nodes used for rendering.
	render::RenderQueue renderQueue;

	cull( renderQueue, node );

	renderDevice->setRenderTarget( target );
	renderDevice->render( renderQueue, this );
}

//-----------------------------------//

void Camera::render( ) const
{
	NodePtr parent = getNode()->getParent();

	if( !parent ) return;

	// Search for the root node.
	while ( parent->getParent() )
	{
		parent = parent->getParent();
	}
	  
	render( parent );
}

//-----------------------------------//

void Camera::cull( render::RenderQueue& queue, NodePtr node ) const
{
	// Let's forget culling for now. Return all renderable nodes.
	// TODO: Check if dynamic_cast is faster than a string comparison.
	
	// Try to see if this is a Group-derived node.
	GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );
	
	// Yes it is.
	if( group )
	{
		// Cull the children nodes recursively.
		foreach( NodePtr child, group->getChildren() )
		{
			if( !child->getVisible() ) continue;
			cull( queue, child );
		}
	}

	// If it is a renderable object, then we perform frustum culling
	// and if the node is visible, then we push it to a list of things
	// to render that will be later passed to the rendering device.
	const std::vector< GeometryPtr >& geometries = node->getGeometry();

	foreach( GeometryPtr geometry, geometries ) 
	{
		// No frustum culling is performed yet.
		// TODO: Hack! :D
		geometry->appendRenderables( queue, node->getTransform() );
	}
}

//-----------------------------------//

const std::string Camera::save( int /*indent*/ )
{
	return "";
}

//-----------------------------------//

const std::string& Camera::getType() const
{
	return type;
}

//-----------------------------------//

} } // end namespaces