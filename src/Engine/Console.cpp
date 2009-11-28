/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
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

void Console::update( float UNUSED(delta) )
{
	// No need to update geometry if the label did not change.
	if( !isDirty ) return;

	buildGeometry();
}

//-----------------------------------//

} } // end namespaces