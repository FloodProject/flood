/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%import "vapor/math/Vector2.h"
%import "vapor/math/Vector4.h"
%include "vapor/math/Vector3.h"

%template(Vector3)  vapor::Vector3T<float>;

%extend vapor::Vector3T<float>
{
	const char* __str__() 
	{
		static char temp[256];
		sprintf(temp,"[%g, %g, %g]", $self->x, $self->y, $self->z);
		return &temp[0];
	}
}

//-----------------------------------//

%warnfilter(503,509) Color;
%include "vapor/math/Color.h"

%extend vapor::Color
{
	const char* __str__() 
	{
		static char temp[256];
		sprintf(temp,"[%g, %g, %g, %g]", $self->r, $self->g, $self->b, $self->a);
		return &temp[0];
	}
}

//-----------------------------------//

%include "vapor/math/EulerAngles.h"

%extend vapor::EulerAngles
{
	const char* __str__() 
	{
		static char temp[256];
		sprintf(temp,"[%g, %g, %g]", $self->x, $self->y, $self->z);
		return &temp[0];
	}
}

//-----------------------------------//

%include "vapor/math/Quaternion.h"

%extend vapor::Quaternion
{
	const char* __str__() 
	{
		static char temp[256];
		sprintf(temp,"[%g, %g, %g, %g]", $self->x, $self->y, $self->z, $self->w);
		return &temp[0];
	}
}

//-----------------------------------//

%include "vapor/math/Ray.h"

%import "vapor/math/Matrix4x4.h"
%include "vapor/math/Matrix4x3.h"

%include "vapor/math/BoundingBox.h"

//%include "vapor/math/Plane.h"
//%include "vapor/math/Frustum.h"
