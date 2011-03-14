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

BEGIN_CLASS_PARENT_ABSTRACT(Controller, Component)
	FIELD_PRIMITIVE(bool, enabled)
END_CLASS()

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