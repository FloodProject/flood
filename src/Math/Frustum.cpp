/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Memory.h"
#include "Math/Frustum.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_ENUM(Projection)
	ENUM(Orthographic)
	ENUM(Perspective)
REFLECT_ENUM_END()

REFLECT_CLASS(Frustum)
	FIELD_ENUM(0, Projection, projection)
	FIELD_PRIMITIVE(1, float, fieldOfView)
	FIELD_PRIMITIVE(2, float, nearPlane)
	FIELD_PRIMITIVE(3, float, farPlane)
	FIELD_PRIMITIVE(4, float, aspectRatio)
	FIELD_READONLY(aspectRatio)
	FIELD_NO_SERIALIZE(aspectRatio)
	FIELD_PRIMITIVE(5, Vector3, orthoSize)
REFLECT_CLASS_END()

//-----------------------------------//

Frustum::Frustum()
	: projection(Projection::Perspective)
	, fieldOfView(60)
	, nearPlane(0.1)
	, farPlane(100)
	, aspectRatio(1)
	, orthoSize(50, 50, 0)
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
	for(size_t i = 0; i < ARRAY_SIZE(rhs.planes); i++ )
		planes[i] = rhs.planes[i];
}

//-----------------------------------//

void Frustum::updateProjection()
{
	switch(projection)
	{
	case Projection::Perspective:
		matProjection = Matrix4x4::createPerspective( fieldOfView, aspectRatio, nearPlane, farPlane );
		break;
	case Projection::Orthographic:
		matProjection = Matrix4x4::createOrthographic( 0, orthoSize.x, 0, orthoSize.y, nearPlane, farPlane );
		break;
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

static const Vector3 cornerPoints[] =
{
	Vector3(-1,  1,  1), Vector3(1,  1,  1),
	Vector3(-1, -1,  1), Vector3(1, -1,  1),
	Vector3(-1,  1, -1), Vector3(1,  1, -1),
	Vector3(-1, -1, -1), Vector3(1, -1, -1)
};

void Frustum::updateCorners( const Matrix4x3& matView )
{
	Matrix4x4 matClip = /*Matrix4x4(matView) **/ matProjection;
	Matrix4x4 matInvClip = matClip.inverse();

	// The following are the corner points of the frustum (which becomes
	// a unit cube) in clip space. Check the diagram for more information:
	// http://www.songho.ca/opengl/files/gl_projectionmatrix01.png

	for(size_t i = 0; i < ARRAY_SIZE(cornerPoints); i++)
	{
		const Vector3& corner = cornerPoints[i];

		Vector4 c = matInvClip * Vector4(corner, 1.0);
		corners[i] = Vector3(c.x / c.w, c.y / c.w, c.z / c.w);
	}
}

//-----------------------------------//

bool Frustum::intersects( const BoundingBox& box ) const
{
	// Loop through each side of the frustum and test if the box lies outside any of them.
	
	for(size_t i = 0; i < ARRAY_SIZE(planes); i++)
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

NAMESPACE_CORE_END