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

BEGIN_CLASS_PARENT(Console, Overlay)
END_CLASS()

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