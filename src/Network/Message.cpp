/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/SerializationHelpers.h"

#include "Network/Message.h"
#include "Network/Session.h"
#include "Network/Peer.h"
#include "Network/Network.h"

#include <enet/enet.h>
#include <FastLZ/fastlz.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Message::Message(MessageId id)
	: id(id)
	, ms(nullptr)
	, packet(nullptr)
	, flags(0)
{
	ms = StreamCreateFromMemory(AllocatorGetThis(), 0);

	SetBitFlag(flags, MessageFlags::Reliable, true);
	SetBitFlag(flags, MessageFlags::Binary, true);
	SetBitFlag(flags, MessageFlags::Compressed, true);

	EncodeVariableInteger(ms, id);
	StreamWrite(ms, &flags, sizeof(flags));
}

//-----------------------------------//

Message::~Message()
{
	Deallocate(ms);
}

//-----------------------------------//

void Message::createPacket()
{
	void* data = nullptr;
	size_t size = 0;
	uint32 flags = 0;

	if(GetBitFlag(flags, MessageFlags::Reliable))
		flags |= ENET_PACKET_FLAG_RELIABLE;

	packet = enet_packet_create(nullptr, 0, flags);
}

//-----------------------------------//

static size_t GetCompressionBufferSize(size_t size)
{
	// From the FastLZ docs: "The output buffer must be at least 5%
	// larger than the input buffer and can not be smaller than 66 bytes."

	size_t bufSize = size + size_t(size * 0.05f);
	if(bufSize < 66) bufSize = 128;

	return bufSize;
}

void Message::prepare()
{
	if( !packet ) createPacket();

	size_t size = (size_t) ms->position;
	enet_packet_resize(packet, size);

	if( GetBitFlag(flags, MessageFlags::Compressed) )
	{
		size_t bufSize = GetCompressionBufferSize(size);
		enet_packet_resize(packet, bufSize);

		int32 length = fastlz_compress_level(0, &ms->data[0], size, packet->data);
		packet->dataLength = length;
	}
	else
	{
		enet_packet_resize(packet, size);
		memcpy(packet->data, &ms->data[0], size);
	}
}

//-----------------------------------//

void Message::setPacket(ENetPacket* packet)
{
	if( GetBitFlag(flags, MessageFlags::Compressed) )
	{
		size_t bufSize = GetCompressionBufferSize(packet->dataLength);
		StreamResize(ms, bufSize);

		int32 length = fastlz_decompress(packet->data,
			packet->dataLength, &ms->data[0], ms->data.size());

		StreamResize(ms, length);
	}
	else
	{
		StreamResize(ms, packet->dataLength);
		StreamWrite(ms, packet->data, packet->dataLength);
	}
}

//-----------------------------------//

MessagePtr MessageCreate(MessageId id)
{
	MessagePtr message = Allocate(Message, AllocatorGetNetwork(), id);
	return message;
}

//-----------------------------------//

NAMESPACE_CORE_END