/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
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