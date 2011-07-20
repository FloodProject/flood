/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Plugin.h"

FWD_DECL_INTRUSIVE(Session)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct MessageHandler;
typedef std::vector<MessageHandler> MessagesTable;

REFLECT_DECLARE_CLASS(MessagePlugin)

class API_CORE MessagePlugin : public Plugin
{
	REFLECT_DECLARE_OBJECT(MessagePlugin)

	friend class PluginManager;

public:

	MessagePlugin();
	virtual ~MessagePlugin();

	// Gets the messages that this plugin handles.
	virtual const MessagesTable& getMessagesTable() = 0;

	// Gets the enum with the messages that this plugin handles.
	virtual Enum* getMessagesEnum() = 0;
};

//-----------------------------------//

NAMESPACE_CORE_END