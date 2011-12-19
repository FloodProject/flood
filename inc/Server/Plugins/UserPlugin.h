/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Server/API.h"
#include "Server/ServerPlugin.h"
#include "Protocol/UserMessages.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(UserPlugin)

class UserPlugin : public UserMessageHandler
{
	REFLECT_DECLARE_OBJECT(UserPlugin)

public:

	void handleUserAuth(const SessionPtr&, const UserAuthMessage&) OVERRIDE;
	void handleUserChangeName(const SessionPtr&, const UserChangeNameMessage&) OVERRIDE;
};

//-----------------------------------//

NAMESPACE_SERVER_END