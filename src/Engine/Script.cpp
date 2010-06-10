/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

//#ifdef VAPOR_SCRIPTING_LUA

#include "vapor/script/Script.h"
#include "vapor/script/State.h"

namespace vapor {

//-----------------------------------//

void Script::execute()
{
	state->execute( this );
}

//-----------------------------------//

} // end namespace

//#endif