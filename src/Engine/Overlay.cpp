/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/gui/Overlay.h"
#include "vapor/scene/Entity.h"
#include "vapor/scene/Transform.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Overlay, Transform)
END_CLASS()

//-----------------------------------//

Overlay::Overlay() 
	: positioning( Positioning::Relative ),
	anchor( Anchor::TopLeft )
{ }

//-----------------------------------//

void Overlay::update( double )
{
	TransformPtr transform = getEntity()->getTransform();
	assert( transform != nullptr );
	transform->reset();
	
	Vector3 move;

	switch(anchor)
	{
	case Anchor::TopLeft:
		move.x = 20.0f;
		move.y = 20.0f;
		break;
	}

	transform->translate(move);
}

//-----------------------------------//

void Overlay::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

//-----------------------------------//

} // end namespace