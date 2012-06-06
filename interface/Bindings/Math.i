/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

%ignore Vector2P;
%ignore Vector3P;

%extend Vector2
{
	%rename(SetZero) zero;

	%rename(Add) operator+;
	%rename(Subtract) operator-;
	%rename(Multiply) operator*;
	%rename(Divide) operator/;
	%rename(Greater) operator>;
	%rename(Lesser) operator<;
	%rename(Equals) operator==;
	%ignore operator =;
	%ignore operator !=;
	%ignore operator +=;
	%ignore operator -=;
	%ignore operator *=;
	%ignore operator /=;
}

%extend Vector2i
{
	%rename(SetZero) zero;

	%rename(Add) operator+;
	%rename(Subtract) operator-;
	%rename(Multiply) operator*;
	%rename(Divide) operator/;
	%rename(Greater) operator>;
	%rename(Lesser) operator<;
	%rename(Equals) operator==;
	%ignore operator =;
	%ignore operator !=;
	%ignore operator +=;
	%ignore operator -=;
	%ignore operator *=;
	%ignore operator /=;
}

%extend Vector3
{
	%rename(SetZero) zero;

	%rename(Add) operator+;
	%rename(Subtract) operator-;
	%rename(Multiply) operator*;
	%rename(Divide) operator/;
	%rename(Greater) operator>;
	%rename(Lesser) operator<;
	%rename(Equals) operator==;
	%ignore operator =;
	%ignore operator !=;
	%ignore operator +=;
	%ignore operator -=;
	%ignore operator *=;
	%ignore operator /=;
	
	const char* ToString() 
	{
		static char temp[256];
		sprintf(temp,"[%f, %f, %f]", $self->x, $self->y, $self->z);
		return &temp[0];
	}
}

%extend Vector4
{
	%rename(SetZero) zero;

	%rename(Add) operator+;
	%rename(Subtract) operator-;
	%rename(Multiply) operator*;
	%rename(Divide) operator/;
	%rename(Greater) operator>;
	%rename(Lesser) operator<;
	%rename(Equals) operator==;
	%ignore operator =;
	%ignore operator !=;
	%ignore operator +=;
	%ignore operator -=;
	%ignore operator *=;
	%ignore operator /=;
}

%include "Math/Vector.h"

//-----------------------------------//

%ignore ColorP;
%extend Color
{
	const char* ToString() 
	{
		static char temp[256];
		sprintf(temp,"[%f, %f, %f, %f]", $self->r, $self->g, $self->b, $self->a);
		return &temp[0];
	}
}

%warnfilter(503,509) Color;
%include "Math/Color.h"

//-----------------------------------//

%ignore EulerAnglesP;
%extend EulerAngles
{
	const char* ToString() 
	{
		static char temp[256];
		sprintf(temp,"[%f, %f, %f]", $self->x, $self->y, $self->z);
		return &temp[0];
	}
}

%include "Math/EulerAngles.h"

//-----------------------------------//

%ignore QuaternionP;
%extend Quaternion
{
	%rename(SetIdentity) identity;

	const char* ToString() 
	{
		static char temp[256];
		sprintf(temp,"[%f, %f, %f, %f]", $self->x, $self->y, $self->z, $self->w);
		return &temp[0];
	}
}

%include "Math/Quaternion.h"

//-----------------------------------//

%include "Math/Ray.h"

//-----------------------------------//

%extend Matrix4x3
{
	%rename(SetIdentity) identity;
}

%extend Matrix4x4
{
	%rename(SetIdentity) identity;
}

%include "Math/Matrix4x3.h"
%include "Math/Matrix4x4.h"

//-----------------------------------//

%include "Math/BoundingBox.h"

%extend BoundingBox
{
	const char* ToString() 
	{
		static char temp[256];
		sprintf(temp,"[min: (%.2f, %.2f, %.2f), max: (%.2f, %.2f, %.2f)]",
		 $self->min.x, $self->min.y, $self->min.z,
		 $self->max.x, $self->max.y, $self->max.z);
		return &temp[0];
	}
}

//-----------------------------------//

//%include "Math/Plane.h"

//-----------------------------------//

%include "Math/Frustum.h"
