/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Extension.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Extension, Object)
REFLECT_CLASS_END()

//-----------------------------------//

Extension::Extension()
{
}

//-----------------------------------//

Extension::~Extension()
{
}

//-----------------------------------//

void Extension::onInit()
{
}

//-----------------------------------//

void Extension::onCleanup()
{
}

//-----------------------------------//

NAMESPACE_CORE_END