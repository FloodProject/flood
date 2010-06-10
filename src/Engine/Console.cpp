/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/gui/Console.h"
#include "vapor/render/Quad.h"

namespace vapor {

//-----------------------------------//

const std::string& Console::type = "Console";

//-----------------------------------//

Console::Console()
{
	buildGeometry();
}

//-----------------------------------//

void Console::buildGeometry()
{
	// No need to update geometry if the label did not change.
	if( !isDirty ) return;
}

//-----------------------------------//

void Console::update( double VAPOR_UNUSED(delta) )
{
	// No need to update geometry if the label did not change.
	if( !isDirty ) return;

	buildGeometry();
}

//-----------------------------------//

const std::string& Console::getType() const 
{
	return type; 
}

//-----------------------------------//

} // end namespace