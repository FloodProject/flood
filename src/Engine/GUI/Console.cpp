/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "GUI/Console.h"
#include "Geometry/Quad.h"

NAMESPACE_ENGINE_BEGIN

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

NAMESPACE_ENGINE_END