/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_PHYSICS_BULLET

#include "physics/Shape.h"
#include "physics/Body.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT_ABSTRACT(Shape, Component)
END_CLASS()

//-----------------------------------//

Shape::~Shape()
{ }

//-----------------------------------//

void Shape::removeBody()
{
	const BodyPtr& body = weakBody.lock();

	if( body )
		body->removeWorld();
}

//-----------------------------------//

} // end namespace

#endif