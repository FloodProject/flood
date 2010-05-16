/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/gui/Overlay.h"

namespace vapor { namespace gui {

const std::string& Overlay::type = "Overlay";

//-----------------------------------//

Overlay::Overlay() 
	: positioning( Positioning::Absolute ),
	anchor( Anchor::TopLeft )
{ }

//-----------------------------------//

void Overlay::update( double VAPOR_UNUSED(delta) )
{

}

//-----------------------------------//

void Overlay::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

//-----------------------------------//

} } // end namespaces