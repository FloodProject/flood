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

namespace vapor { namespace script {

//-----------------------------------//

Script::Script( const std::string& text )
	: text( text )
{

}

//-----------------------------------//

Script::~Script()
{

}

//-----------------------------------//

const std::string& Script::getSource()
{
	return text; 
}

//-----------------------------------//

void Script::setState( State* state )
{
	this->state = state;
}

//-----------------------------------//

void Script::execute()
{
	state->execute( this );
}

//-----------------------------------//

resources::ResourceGroup::Enum Script::getResourceGroup() const
{
	return resources::ResourceGroup::Scripts; 
}

//-----------------------------------//

} } // end namespaces

//#endif