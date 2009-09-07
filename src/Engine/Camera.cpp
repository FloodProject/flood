/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Camera.h"

#include "vapor/scene/Geometry.h"

using namespace vapor::render;

namespace vapor {
	namespace scene {

//-----------------------------------//

Camera::Camera( render::Device* device, Projection::Enum projection )
	: renderDevice( device ), projection( projection ),
		target( device->getRenderTarget() ), fov(45.0f), near_( 0.0f ), far_( 100.0f )
{
	if( !target ) return;

	target->onTargetResize.bind( &Camera::handleTargetResize, this );

	const Settings& settings = target->getSettings();

	width = settings.getWidth();
	height = settings.getHeight();

	setupProjection();
	setupView();
}

//-----------------------------------//

void Camera::lookAt( const math::Vector3& pt )
{

}

//-----------------------------------//

void Camera::setProjection( Projection::Enum projection )
{
	this->projection = projection;
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

void Camera::setFar( float far_ )
{
	this->far_ = far_;
}

//-----------------------------------//

void Camera::setRenderTarget( render::Target* target )
{
	this->target = target;
}

//-----------------------------------//

void Camera::handleTargetResize( const render::Settings& evt )
{
	width = evt.getWidth();
	height = evt.getHeight();

	glViewport( 0, 0, width, height );
}

//-----------------------------------//

void Camera::update()
{
	setupProjection();
	setupView();
}

//-----------------------------------//

void Camera::setupProjection()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	if( projection == Projection::Perspective )
	{
		gluPerspective( fov, getAspectRatio(), near_, far_ );
	}
	else
	{
		//glOrtho( -1.0, 1.0, -1.0, 1.0, near, far );
	}

	glMatrixMode( GL_MODELVIEW );
}

//-----------------------------------//

void Camera::setupView()
{
	glMatrixMode( GL_MODELVIEW );
	
	glLoadIdentity();

	// TODO: parent matrices

	glTranslatef( translation.x, translation.y, translation.z );
}

//-----------------------------------//

float Camera::getAspectRatio() const
{
	return (float) width / height;
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
	renderDevice->render( renderQueue );
}

//-----------------------------------//

void Camera::render( ) const
{
	NodePtr parent = getParent();

	if( !parent ) return;

	// Search for the root node.
	while ( parent->getParent() )
	  parent = parent->getParent();
	  
	render( parent );
}

//-----------------------------------//

void Camera::cull( render::RenderQueue& queue, NodePtr node ) const
{
	// Let's forget culling for now. Return all renderable nodes.

	// TODO: Check if dynamic_cast is faster than a string comparison.
	
	// Try to see if this is a Group-derived node.
	
	try
	{
		GroupPtr group( tr1::dynamic_pointer_cast< Group >( node) );
		
		// Yes it is.
		const std::vector< NodePtr >& children = group->getChildren();

		foreach( NodePtr node, children )
		{
			GeometryPtr geometry( tr1::dynamic_pointer_cast< Geometry >( node) );

			if( !geometry ) continue;

			foreach( RenderablePtr rend, geometry->getRenderables() )
			{
				queue.push_back( rend );
			}
		}
	}
	catch (const std::bad_cast& e)
	{
		// Nope.
		return;
	}    
}

//-----------------------------------//

const std::string Camera::save( int indent )
{
	return "";
}

//-----------------------------------//

const std::string Camera::name() const
{
	return "Camera"; 
}

//-----------------------------------//

//tr1::shared_ptr< Camera > Group::shared_from_this()
//{ 
//	return tr1::static_pointer_cast< Camera >( 
//		Node::shared_from_this() ); 
//}

//-----------------------------------//

} } // end namespaces
