/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

BEGIN_NAMESPACE_EXTERN

//-----------------------------------//

void GetReflectDatabase();

enum ReflectType
{
	ARRAY,
	MAP,
	ENUM,
	CLASS,
};

struct ReflectClass
{
	
};

void ReflectAddType();
void ReflectGetDatabase();
void ReflectCreatePrimitive();

//-----------------------------------//

END_NAMESPACE_EXTERN