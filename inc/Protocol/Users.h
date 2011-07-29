/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/API.h"

FWD_DECL_INTRUSIVE(Session)

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

typedef uint8 UserId;

struct User
{
	User();

	UserId id;
	String name;
	SessionPtr session;
};

typedef std::map<UserId, User> UsersMap;
typedef std::map<SessionPtr, User> UsersSessionMap;

class Users
{
public:

	Users();

	// Gets an id for a new user.
	UserId getNextId();

	// Adds a new user.
	void addUser(const User& user);

	// Removes an user.
	void removeUser(const User& user);

	// Gets an user from its id.
	User* getUserFromId(UserId id);

	// Gets an user from its session.
	User* getUserFromSession(const SessionPtr& session);

	// Gets the session from an id.
	SessionPtr getSessionFromId(UserId id);

	// Sent when a user is added.
	Event1<const User&> onUserAdded;

	// Sent when a user is removed.
	Event1<const User&> onUserRemoved;
	
protected:

	UsersMap users;
	UsersSessionMap sessions;
	UserId nextId;
};

//-----------------------------------//

NAMESPACE_PROTOCOL_END