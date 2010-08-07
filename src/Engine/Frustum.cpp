/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Frustum.h"

namespace vapor {

//-----------------------------------//

//std::string Projection::toString( Projection::Enum proj )
//{
//	switch(proj)
//	{
//	case Projection::Orthographic:
//		return "Ortographic";
//	case Projection::Perspective:
//		return "Perspective";
//	}
//}

//-----------------------------------//

Frustum::Frustum()
	: typeProjection( Projection::Perspective )
	, angleFOV(60)
	, nearPlane(1)
	, farPlane(100)
	, aspectRatio(0)
{ }

//-----------------------------------//

Frustum::Frustum( const Frustum& rhs )
	: typeProjection( rhs.typeProjection )
	, matProjection( rhs.matProjection )
	, angleFOV( rhs.angleFOV )
	, nearPlane( rhs.nearPlane )
	, farPlane( rhs.farPlane )
	, aspectRatio( rhs.aspectRatio )
{ }

//-----------------------------------//

void Frustum::updateProjection( const Vector2i& size )
{
	if( typeProjection == Projection::Perspective )
	{
		matProjection = Matrix4x4::createPerspectiveProjection(
			angleFOV, aspectRatio, nearPlane, farPlane );
	}
	else
	{
		matProjection = Matrix4x4::createOrthographicProjection(
			0, size.x, 0, size.y, nearPlane, farPlane );
	}
}

//-----------------------------------//

void Frustum::updatePlanes( const Matrix4x3& matView )
{
	// Based on paper: "Fast Extraction of Viewing Frustum Planes
	// from the World-View-Projection Matrix" by Gil Gribb.
	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf

	Matrix4x4 matView4( matView );
	Matrix4x4 matClip = matView * matProjection;

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
	Matrix4x4 matInvClip = matProjection.inverse();

	// The following are the corner points of the frustum (which becomes a
	// unit cube) in clip space. Check the diagram for more information:
	// http://www.songho.ca/opengl/files/gl_projectionmatrix01.png

	Vector3 cornerPoints[] =
	{
		Vector3(-1,  1,  1), Vector3( 1,  1,  1),
		Vector3(-1, -1,  1), Vector3( 1, -1,  1),
		Vector3(-1,  1, -1), Vector3( 1,  1, -1),
		Vector3(-1, -1, -1), Vector3( 1, -1, -1)
	};

	int i = 0;
	foreach( const Vector3& corner, cornerPoints )
	{
		Vector4 c = matInvClip * Vector4(corner, 1.0);
		corners[i++] = Vector3(c.x / c.w, c.y / c.w, c.z / c.w);
	}
}

//-----------------------------------//

} // end namespace