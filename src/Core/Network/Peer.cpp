/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/Network/Network.h"
#include "Core/Network/Host.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Session.h"
#include "Core/Network/Packet.h"
#include "Core/Network/PacketProcessor.h"
#include "Core/Network/Processors/PacketKeyExchanger.h"

#if defined(PLATFORM_WINDOWS) && !defined(WIN32)
#define WIN32
#endif

#include <enet/enet.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Peer::Peer()
	: peer(nullptr)
	, host(nullptr)
	, session(nullptr)
	, processors(nullptr)
	, state(PeerState::Disconnected)
{
	processors = Allocate(AllocatorGetNetwork(), PacketProcessors);
}

//-----------------------------------//

void Peer::queuePacket(const PacketPtr& packet, uint8 channel)
{
	if( !packet ) return;

	processors->processOutPacket(this, packet.get(), channel);

	packet->prepare();

	int status = enet_peer_send(getPeer(), channel, packet->getPacket());

	if(status != 0)
	{
		LogWarn("Error sending packet to peer '%s'", getHostName().c_str());
		return;
	}
}

//-----------------------------------//

bool Peer::processInPacket(const PacketPtr& packet, uint8 channel)
{
	return processors->processInPacket(this, packet.get(), channel);
}

//-----------------------------------//

void Peer::addProcessor(PacketProcessor* processor)
{
	processors->addProcessor(processor);
}

//-----------------------------------//

void Peer::disconnect()
{
	//enet_peer_disconnect_now(peer, 0);
	enet_peer_disconnect_later(peer, 0);
	setState(PeerState::Disconnecting);
}

//-----------------------------------//

void Peer::forceDisconnect()
{
	enet_peer_reset(peer);
}

//-----------------------------------//

String Peer::getHostName() const
{
	char name[256];
	
	if(enet_address_get_host(&peer->address, name, ARRAY_SIZE(name)) < 0)
	{
		LogError("Could not get hostname of network peer");
		return "";
	}

	return String(name);
}

//-----------------------------------//

String Peer::getHostIP() const
{
	char ip[64];
	
	if(enet_address_get_host_ip(&peer->address, ip, ARRAY_SIZE(ip)) < 0)
	{
		LogError("Could not get IP address of network peer");
		return "";
	}

	return String(ip);
}

//-----------------------------------//

void Peer::setState(PeerState newState)
{
	if(newState == state)
		return;

	state = newState;
	onStateChanged(state);
}

NAMESPACE_CORE_END