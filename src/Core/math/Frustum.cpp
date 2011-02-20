/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/Frustum.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(Projection)
	ENUM(Orthographic)
	ENUM(Perspective)
END_ENUM()

BEGIN_CLASS(Frustum)
	FIELD_ENUM(Projection, projection)
	FIELD_PRIMITIVE(float, fieldOfView)
	FIELD_PRIMITIVE(float, nearPlane)
	FIELD_PRIMITIVE(float, farPlane)
	FIELD_PRIMITIVE(float, aspectRatio)
END_CLASS()

//-----------------------------------//

Frustum::Frustum()
	: projection( Projection::Perspective )
	, fieldOfView(60)
	, nearPlane(1)
	, farPlane(100)
	, aspectRatio(1)
{ }

//-----------------------------------//

Frustum::Frustum( const Frustum& rhs )
	: projection( rhs.projection )
	, matProjection( rhs.matProjection )
	, fieldOfView( rhs.fieldOfView )
	, nearPlane( rhs.nearPlane )
	, farPlane( rhs.farPlane )
	, aspectRatio( rhs.aspectRatio )
{
	for(uint i = 0; i < VAPOR_ARRAY_SIZE(rhs.planes); i++ )
		planes[i] = rhs.planes[i];
}

//-----------------------------------//

void Frustum::updateProjection( const Vector2i& size )
{
	if( projection == Projection::Perspective )
	{
		matProjection = Matrix4x4::createPerspectiveProjection(
			fieldOfView, aspectRatio, nearPlane, farPlane );
	}
	else
	{
		matProjection = Matrix4x4::createOrthographicProjection(
			0, (float) size.x, 0, (float) size.y, nearPlane, farPlane );
	}
}

//-----------------------------------//

void Frustum::updatePlanes( const Matrix4x3& matView )
{
	// Based on paper: "Fast Extraction of Viewing Frustum Planes
	// from the World-View-Projection Matrix" by Gil Gribb.
	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf

	Matrix4x4 matClip = Matrix4x4(matView) * matProjection;

	// Left clipping plane
	planes[0].normal.x = matClip.m14 + matClip.m11;
	planes[0].normal.y = matClip.m24 + matClip.m21;
	planes[0].normal.z = matClip.m34 + matClip.m31;
	planes[0].offset   = matClip.tw  + matClip.tx;
	planes[0].normalize();

	// Right clipping plane
	planes[1].normal.x = matClip.m14 - matClip.m11;
	planes[1].normal.y = matClip.m24 - matClip.m21;
	planes[1].normal.z = matClip.m34 - matClip.m31;
	planes[1].offset   = matClip.tw  - matClip.tx;
	planes[1].normalize();

	// Top clipping plane
	planes[2].normal.x = matClip.m14 - matClip.m12;
	planes[2].normal.y = matClip.m24 - matClip.m22;
	planes[2].normal.z = matClip.m34 - matClip.m32;
	planes[2].offset   = matClip.tw  - matClip.ty;
	planes[2].normalize();

	// Bottom clipping plane
	planes[3].normal.x = matClip.m14 + matClip.m12;
	planes[3].normal.y = matClip.m24 + matClip.m22;
	planes[3].normal.z = matClip.m34 + matClip.m32;
	planes[3].offset   = matClip.tw  + matClip.ty;
	planes[3].normalize();

	// Near clipping plane
	planes[4].normal.x = matClip.m14 + matClip.m13;
	planes[4].normal.y = matClip.m24 + matClip.m23;
	planes[4].normal.z = matClip.m34 + matClip.m33;
	planes[4].offset   = matClip.tw  + matClip.tz;
	planes[4].normalize();

	// Far clipping plane
	planes[5].normal.x = matClip.m14 - matClip.m13;
	planes[5].normal.y = matClip.m24 - matClip.m23;
	planes[5].normal.z = matClip.m34 - matClip.m33;
	planes[5].offset   = matClip.tw  - matClip.tz;
	planes[5].normalize();

	updateCorners( matView );
}

//-----------------------------------//

void Frustum::updateCorners( const Matrix4x3& matView )
{
	Matrix4x4 matClip = Matrix4x4(matView) * matProjection;
	Matrix4x4 matInvClip = matClip.inverse();

	// The following are the corner points of the frustum (which becomes
	// a unit cube) in clip space. Check the diagram for more information:
	// http://www.songho.ca/opengl/files/gl_projectionmatrix01.png

	Vector3 cornerPoints[] =
	{
		Vector3(-1,  1,  1), Vector3(1,  1,  1),
		Vector3(-1, -1,  1), Vector3(1, -1,  1),
		Vector3(-1,  1, -1), Vector3(1,  1, -1),
		Vector3(-1, -1, -1), Vector3(1, -1, -1)
	};

	for( uint i = 0; i < VAPOR_ARRAY_SIZE(cornerPoints); i++ )
	{
		const Vector3& corner = cornerPoints[i];

		Vector4 c = matInvClip * Vector4(corner, 1);
		corners[i++] = Vector3(c.x / c.w, c.y / c.w, c.z / c.w);
	}
}

//-----------------------------------//

bool Frustum::intersects( const BoundingBox& box ) const
{
	// Loop through each side of the frustum and test if the box lies outside any of them.
	
	for(uint i = 0; i < VAPOR_ARRAY_SIZE(planes); i++)
	{
		const Plane& plane = planes[i];

		if(plane.distance(box.min) >= 0) continue;
		if(plane.distance(box.max.x, box.min.y, box.min.z) >= 0) continue;
		if(plane.distance(box.min.x, box.max.y, box.min.z) >= 0) continue;
		if(plane.distance(box.max.x, box.max.y, box.min.z) >= 0) continue;
		if(plane.distance(box.min.x, box.min.y, box.max.z) >= 0) continue;
		if(plane.distance(box.max.x, box.min.y, box.max.z) >= 0) continue;
		if(plane.distance(box.min.x, box.max.y, box.max.z) >= 0) continue;
		if(plane.distance(box.max) >= 0) continue;
    
		return false;
	}

	return true;
}

//-----------------------------------//

} // end namespace