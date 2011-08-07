/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"
#include "Network/MessagePlugin.h"
#include "Protocol/Users.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_DECLARE_ENUM(UserAuthType)

enum API_PROTOCOL UserAuthType
{
	None,
	Password,
	Certificate
};

REFLECT_DECLARE_CLASS(UserAuthMessage)

struct API_PROTOCOL UserAuthMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(UserAuthMessage)
	String name;
	UserAuthType type;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(UserJoinMessage)

struct API_PROTOCOL UserJoinMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(UserJoinMessage)
	UserId user;
	String name;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(UserLeaveMessage)

struct API_PROTOCOL UserLeaveMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(UserLeaveMessage)
	UserId user;
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(UserChangeNameMessage)

struct API_PROTOCOL UserChangeNameMessage : MessageDefinition
{
	REFLECT_DECLARE_OBJECT(UserChangeNameMessage)
	UserId user;
	String name;
};

//-----------------------------------//

#define USER_INITIAL_ID 600

REFLECT_DECLARE_ENUM(UserMessageIds)

struct API_PROTOCOL UserMessageIds
{
	enum Enum : MessageId
	{
		UserAuth		= USER_INITIAL_ID + 0,
		UserChangeName	= USER_INITIAL_ID + 1,
		UserJoin		= USER_INITIAL_ID + 2,
		UserLeave		= USER_INITIAL_ID + 3,
		UserNotifyName	= USER_INITIAL_ID + 4,
	};
};

//-----------------------------------//

REFLECT_DECLARE_CLASS(UserMessagePlugin)

class API_PROTOCOL UserMessagePlugin : public MessagePlugin
{
	REFLECT_DECLARE_OBJECT(UserMessagePlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Gets the messages that this plugin handles.
	const MessagesTable& getMessagesTable() OVERRIDE;

	// Gets the enum with the messages that this plugin handles.
	Enum* getMessagesEnum() OVERRIDE;

	// Message callbacks.
	virtual void handleUserAuth(const SessionPtr&, const UserAuthMessage&) {}
	virtual void handleUserChangeName(const SessionPtr&, const UserChangeNameMessage&) {}
	virtual void handleUserJoin(const SessionPtr&, const UserJoinMessage&) {}
	virtual void handleUserLeave(const SessionPtr&, const UserLeaveMessage&) {}
	//virtual void handleUserNotifyName(const SessionPtr&, const UserNotifyNameMessage&) {}

	Users users;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END