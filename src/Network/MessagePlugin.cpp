/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/MessagePlugin.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(MessagePlugin, Plugin)
REFLECT_CLASS_END()

//-----------------------------------//

MessagePlugin::MessagePlugin()
{
}

//-----------------------------------//

MessagePlugin::~MessagePlugin()
{
}

//-----------------------------------//

NAMESPACE_CORE_END