/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Controllers/Controller.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Controller, Component)
	FIELD_PRIMITIVE(4, bool, enabled)
REFLECT_CLASS_END()

//-----------------------------------//

Controller::Controller()
{ }

//-----------------------------------//

Controller::~Controller()
{ }

//-----------------------------------//

void Controller::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

//-----------------------------------//

void Controller::update( float time )
{
	if( !enabled ) return;

	_update(time);
}

//-----------------------------------//

NAMESPACE_ENGINE_END