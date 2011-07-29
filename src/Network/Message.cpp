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
#include "Network/Network.h"
#include <enet/enet.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Message::Message()
	: id(0)
	, type(MessageType::Reliable)
	, packet(nullptr)
	, index(0)
{
}

//-----------------------------------//

Message::~Message()
{
}

//-----------------------------------//

void Message::createPacket()
{
	void* data = nullptr;
	size_t size = 0;
	uint32 flags = /*ENET_PACKET_FLAG_NO_ALLOCATE*/0;

	if(type == MessageType::Reliable)
		flags |= ENET_PACKET_FLAG_RELIABLE;

	packet = enet_packet_create(nullptr, 0, flags);
}

//-----------------------------------//

void Message::prepare()
{
	enet_packet_resize(packet, index);
	memcpy(packet->data, &data.front(), index);
	
	//packet->data = &data.front();
	//packet->dataLength = index;
}

//-----------------------------------//

void Message::setPacket(ENetPacket* packet)
{
	data.resize(packet->dataLength);
	memcpy(&data.front(), packet->data, packet->dataLength);
}

//-----------------------------------//

MessagePtr MessageCreate(MessageId id)
{
	MessagePtr message = Allocate(Message, AllocatorGetNetwork());
	message->createPacket();
	message->write(id);

	return message;
}

//-----------------------------------//

NAMESPACE_CORE_END