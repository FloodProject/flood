/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Protocol/Users.h"
#include "Network/Session.h"
#include "Network/Peer.h"
#include "Core/Log.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

User::User()
	: id(0)
	, session(nullptr)
{
}

//-----------------------------------//

Users::Users()
	: nextId(0)
{
}

//-----------------------------------//

UserId Users::getNextId()
{
	return ++nextId;
}

//-----------------------------------//

void Users::addUser(const User& user)
{
	users[user.id] = user;
	sessions[user.session] = user;

	onUserAdded(user);
}

//-----------------------------------//

void Users::removeUser(const User& user)
{
	UsersMap::iterator it = users.find(user.id);
	
	if( it == users.end() )
		return;

	UsersSessionMap::iterator it2 = sessions.find(user.session);

	users.erase(it);
	sessions.erase(it2);
}

//-----------------------------------//

User* Users::getUserFromId(UserId id)
{
	UsersMap::iterator it = users.find(id);

	if( it == users.end() )
		return nullptr;

	return &it->second;
}

//-----------------------------------//

User* Users::getUserFromSession(const SessionPtr& session)
{
	UsersSessionMap::iterator it = sessions.find(session);

	if( it == sessions.end() )
		return nullptr;

	return &it->second;
}

//-----------------------------------//

SessionPtr Users::getSessionFromId(UserId id)
{
	User* user = getUserFromId(id);
	if( !user ) return nullptr;

	return user->session;
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END