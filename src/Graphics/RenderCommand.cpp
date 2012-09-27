/************************************************************************
*
*  Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderCommand.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderCommand::RenderCommand(RenderCommandType type)
	: type(type)
{
}

DrawCallCommand::DrawCallCommand()
	: RenderCommand(RenderCommandType::DrawCall)
{
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END