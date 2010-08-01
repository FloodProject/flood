/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Viewport.h"
#include "vapor/render/Target.h"
#include "vapor/scene/Camera.h"

namespace vapor {

//-----------------------------------//

Viewport::Viewport( CameraPtr camera, RenderTarget* target )
	: weakCamera( camera )
	, target( target )
{
	setClearColor( Color::White );
	setRenderTarget( target );
}

//-----------------------------------//

void Viewport::setRenderTarget( RenderTarget* newTarget )
{
	//if( target ) // Remove the old target resize notification.
	//	target->onTargetResize -= fd::bind( &Viewport::handleTargetResize, this );

	//target = newTarget;
	//target->onTargetResize += fd::bind( &Viewport::handleTargetResize, this );
	//handleTargetResize( target->getSettings() );
}

//-----------------------------------//

Vector3 Viewport::Unproject( const Vector3& screen, const Matrix4x4& projection, const Matrix4x3& view ) const
{
	Matrix4x4 view4( view );
	Matrix4x4 inverseVP = (view4*projection).inverse();

	Vector2i size = getSize();

    // Map x and y from window coordinates, map to range -1 to 1 

    Vector4 pos;
    pos.x = (screen.x /*- offset.x*/) / float(size.x) * 2.0f - 1.0f;
    pos.y = (screen.y /*- offset.y*/) / float(size.y) * 2.0f - 1.0f;
    pos.z = screen.z * 2.0f - 1.0f;
    pos.w = 1.0f;
 
	Vector4 pos2 = inverseVP * pos;
	Vector3 pos_out( pos2.x, pos2.y, pos2.z );
 
    return pos_out / pos2.w;
}

//-----------------------------------//

float Viewport::getAspectRatio() const
{
	const Vector2i size = getSize();

	if( size.y == 0 )
		return 0.0f;
	
	return float(size.x) / size.y;
}

//-----------------------------------//

Vector2i Viewport::getSize() const
{
	//if( size == Vector2i(-1, -1) )
		return target->getSettings().getSize();
	//else
		//return size;
}

//-----------------------------------//

bool Viewport::operator < (Viewport& v)
{
	return getPriority() < v.getPriority();
}

//-----------------------------------//

void Viewport::update()
{
	CameraPtr camera( weakCamera );
	camera->setViewport( this );
	camera->render();
}

//-----------------------------------//

} // end namespace