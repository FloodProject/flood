/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/Message.h"
#include "Network/Session.h"
#include "Network/Peer.h"
#include <enet/enet.h>

NAMESPACE_BEGIN

//-----------------------------------//

Message::Message()
	: id(0)
	, type(MessageType::Reliable)
	, packet(nullptr)
{
	createPacket();
}

//-----------------------------------//

Message::~Message()
{
	//enet_packet_destroy(packet);
}

//-----------------------------------//

void Message::createPacket()
{
	void* data = nullptr;
	size_t size = 0;
	uint32 flags = ENET_PACKET_FLAG_NO_ALLOCATE;

	if(type == MessageType::Reliable)
		flags |= ENET_PACKET_FLAG_RELIABLE;

	packet = enet_packet_create(nullptr, 0, flags);
}

//-----------------------------------//

void Message::prepare()
{
	packet->data = &data.front();
	packet->dataLength = data.size();
}

//-----------------------------------//

NAMESPACE_END