/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Controllers/Controller.h"

namespace vapor {

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Controller, Component)
	FIELD_PRIMITIVE(bool, enabled)
REFLECT_CLASS_END()

//-----------------------------------//

Controller::Controller()
{ }

//-----------------------------------//

Controller::~Controller()
{ }

//-----------------------------------//

void Controller::update( float time )
{
	if( !enabled )
		return;

	_update(time);
}

//-----------------------------------//

} // end namespace