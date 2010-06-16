/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/gui/Overlay.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"

namespace vapor {

const std::string& Overlay::type = "Overlay";

//-----------------------------------//

Overlay::Overlay() 
	: positioning( Positioning::Relative ),
	anchor( Anchor::TopLeft )
{ }

//-----------------------------------//

void Overlay::update( double VAPOR_UNUSED(delta) )
{
	TransformPtr transform = getNode()->getTransform();
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