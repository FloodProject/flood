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

using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::math;

namespace vapor { namespace gui {

//-----------------------------------//

const std::string& Console::type = "Console";

//-----------------------------------//

Console::Console()
{
	buildGeometry();
}

//-----------------------------------//

Console::~Console()
{

}

//-----------------------------------//

void Console::buildGeometry()
{
	// No need to update geometry if the label did not change.
	if( !isDirty ) return;
}

//-----------------------------------//

void Console::update( double UNUSED(delta) )
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

const std::string Console::save( int indent )
{
	return ""; 
}

//-----------------------------------//

} } // end namespaces