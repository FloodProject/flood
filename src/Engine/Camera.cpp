/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Camera.h"
#include "vapor/scene/Group.h"
#include "vapor/scene/Geometry.h"

using namespace vapor::math;
using namespace vapor::render;

namespace vapor {
	namespace scene {

//-----------------------------------//

Camera::Camera( render::Device* device, Projection::Enum projection )
	: renderDevice( device ), projection( projection ),
		target( nullptr ), fov(45.0f), near_( 1.0f ), far_( 10000.0f ),
		lookAtVector( Vector3( 0.0f, 0.0f, 0.0f ) ), 
		upVector( Vector3( 0.0f, 1.0f, 0.0f ) )
{
	setRenderTarget( device->getRenderTarget() );
}

//-----------------------------------//

void Camera::lookAt( const math::Vector3& pt )
{
	lookAtVector = pt;
}

//-----------------------------------//

void Camera::setProjection( Projection::Enum projection )
{
	this->projection = projection;
}

//-----------------------------------//

const math::Matrix4& Camera::getProjectionMatrix() const
{
	return Matrix4::Identity;
}

//-----------------------------------//

const math::Matrix4& Camera::getViewMatrix() const
{
	return viewMatrix;
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

void Camera::setRenderTarget( render::Target* newTarget )
{
	if( !newTarget ) return;
	
	// TODO: remove only this from the delegate
	//target->onTargetResize.clear();

	target = newTarget;
	target->onTargetResize.bind( &Camera::handleTargetResize, this );

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

void Camera::update( double delta )
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
		glOrtho( -1.0, 1.0, -1.0, 1.0, near_, far_ );
	}
}

//-----------------------------------//

void Camera::setupView()
{
	Vector3 eye = Vector3( absoluteLocalToWorld.tx, 
		absoluteLocalToWorld.ty, absoluteLocalToWorld.tz );

	Vector3 zaxis = ( lookAtVector - eye ).normalize();
	Vector3	xaxis = upVector.cross(zaxis).normalize();
	Vector3	yaxis = zaxis.cross(xaxis);

	Matrix4 m;
	m.m11 = xaxis.x;
	m.m12 = xaxis.y;
	m.m13 = xaxis.z;

	m.m21 = yaxis.x;
	m.m22 = yaxis.y;
	m.m23 = yaxis.z;

	m.m31 = zaxis.x;
	m.m32 = zaxis.y;
	m.m33 = zaxis.z;

	m.tx = -xaxis.dot(eye);
	m.ty = -yaxis.dot(eye);
	m.tz = -zaxis.dot(eye);

	viewMatrix = m * getLocalTransform();

	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity();

	//gluLookAt( eye.x, eye.y, eye.z,
	//	lookAtVector.x, lookAtVector.y, lookAtVector.z,
	//	upVector.x, upVector.y, upVector.z );

	//float test[16];
	//glGetFloatv( GL_MODELVIEW_MATRIX, test );

	//Matrix4 glView;

	//glView.m11 = test[0];
	//glView.m12 = test[1];
	//glView.m13 = test[2];
	//
	//glView.m21 = test[4];
	//glView.m22 = test[5];
	//glView.m23 = test[6];

	//glView.m31 = test[8];
	//glView.m32 = test[9];
	//glView.m33 = test[10];

	//glView.tx = test[12];
	//glView.ty = test[13];
	//glView.tz = test[14];

	viewMatrix = absoluteLocalToWorld;
}

//-----------------------------------//

float Camera::getAspectRatio() const
{
	if( height == 0) return 0.0f;
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
	renderDevice->render( renderQueue, getViewMatrix() );
}

//-----------------------------------//

void Camera::render( ) const
{
	NodePtr parent = getParent();

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
	GroupPtr group( tr1::dynamic_pointer_cast< Group >( node) );
	
	// Yes it is.
	if( group )
	{
		const std::vector< NodePtr >& children = group->getChildren();

		// Cull the children nodes recursively.
		foreach( NodePtr child, group->getChildren() )
		{
			cull( queue, child );
		}
	}

	// If it is a renderable object, then we perform frustum culling
	// and if the node is visible, then we push it to a list of things
	// to render that will be later passed to the rendering device.
	GeometryPtr geometry( tr1::dynamic_pointer_cast< Geometry >( node) );

	if( geometry ) 
	{
		// No frustum culling is performed yet.
		foreach( RenderablePtr rend, geometry->getRenderables() )
		{
			RenderState renderState;
			
			renderState.renderable = rend;
			renderState.modelMatrix = geometry->getAbsoluteTransform();

			queue.push_back( renderState );
		}
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

} } // end namespaces