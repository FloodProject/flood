/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_PHYSICS_BULLET

#include "Engine/Physics/Shape.h"
#include "Engine/Physics/Body.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Shape, Component)
REFLECT_CLASS_END()

//-----------------------------------//

Shape::~Shape()
{
}

//-----------------------------------//

void Shape::removeBody()
{
	if( !body ) return;

	body->removeWorld();
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif