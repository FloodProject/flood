/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Camera.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Camera::Camera( render::Device* device, Projection::Enum projection )
	: renderDevice( device ), projection( projection ),
		target( device->getRenderTarget() ), near_( 0.0f ), far_( 100.0f )
{
	if( target ) 
	{
		target->onTargetResize.bind( &Camera::handleTargetResize, this );
	}
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
}

//-----------------------------------//

void Camera::setupProjection()
{
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	gluPerspective( fov, getAspectRatio(), near_, far_ );

	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
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
//  NodePtr parent( getParent() );
//  
//  while ( parent->getParent() )
//	  parent = parent->getParent();
//      
//  render( parent );
}

//-----------------------------------//

void Camera::cull( render::RenderQueue& queue, NodePtr root ) const
{

}

//-----------------------------------//

std::string Camera::save( int indent )
{
	return "";
}

//-----------------------------------//

} } // end namespaces
