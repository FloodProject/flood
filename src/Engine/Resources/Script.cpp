/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Script/Script.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Script, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

//void Script::execute()
//{
//	if( !state )
//		return;
//
//	state->execute( this );
//}

//-----------------------------------//

NAMESPACE_ENGINE_END
