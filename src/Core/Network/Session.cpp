/************************************************************************
*
* vapor3D Core © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Session.h"
#include "Core/Network/Peer.h"
#include "Core/Timer.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Session::Session()
 : lastConnection(0)
 , lastCommunication(0)
{
}

//-----------------------------------//

Session::~Session()
{
}

//-----------------------------------//

void Session::handleAuthentication()
{

}

//-----------------------------------//

void Session::handleConnection()
{
	lastConnection = TimerGetCurrentTimeMs();
}

//-----------------------------------//

NAMESPACE_CORE_END