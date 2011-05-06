/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Message.h"

NAMESPACE_BEGIN

//-----------------------------------//

class MessageHandler
{
public:

	virtual ~MessageHandler() { }

	// Handles the message.
	virtual void handleMessage(const MessagePtr&) = 0;
};

//-----------------------------------//

NAMESPACE_END