/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%import "Math/Vector.h"
%import "Math/Vector.h"
%include "Math/Vector.h"

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
%include "Math/Color.h"

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

%include "Math/EulerAngles.h"

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

%include "Math/Quaternion.h"

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

%include "Math/Ray.h"

%import "Math/Matrix4x4.h"
%include "Math/Matrix4x3.h"

%include "Math/BoundingBox.h"

//%include "Math/Plane.h"
%import "Math/Frustum.h"
