/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "gui/Console.h"
#include "Render/Quad.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Console, Overlay)
REFLECT_CLASS_END()

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

void Console::update( float )
{
	// No need to update geometry if the label did not change.
	if( !isDirty ) return;

	buildGeometry();
}

//-----------------------------------//

} // end namespace