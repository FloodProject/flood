/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core.h"
#include "vapor/Task.h"

namespace vapor {

//-----------------------------------//

Task::Task()
	: finish(false)
{ }

//-----------------------------------//

void Task::flagFinish()
{
	finish = true;
}

//-----------------------------------//

} // end namespace