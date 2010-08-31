/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/controllers/Controller.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT_ABSTRACT(Controller, Component)
	FIELD_PRIMITIVE(Controller, bool, enabled)
END_CLASS()

//-----------------------------------//

Controller::Controller()
{ }

//-----------------------------------//

Controller::~Controller()
{ }

//-----------------------------------//

} // end namespace