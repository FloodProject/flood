/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#ifdef ENABLE_NETWORK_ENET

#include "Core/Stream.h"
#include "Core/SerializationHelpers.h"

#include "Core/Network/Message.h"
#include "Core/Network/Session.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Network.h"

#define NOMINMAX
#include <enet/enet.h>

#include <fastlz.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Message::Message(MessageId id)
	: id(id)
	, ms(nullptr)
	, packet(nullptr)
	, freePacket(false)
	, flags(0)
{
	ms = StreamCreateFromMemory(AllocatorGetThis(), 512);
	SetBitFlag(flags, MessageFlags::Reliable, true);
	//SetBitFlag(flags, MessageFlags::Compressed, true);
}

//-----------------------------------//

Message::~Message()
{
	Deallocate(ms);

	if(freePacket)
	{
		enet_packet_destroy(packet);
		packet = nullptr;
	}
}

//-----------------------------------//

void Message::createPacket()
{
	void* data = nullptr;
	size_t size = 0;
	uint32 pflags = 0;

	if(GetBitFlag(flags, MessageFlags::Reliable))
	{
		pflags |= ENET_PACKET_FLAG_RELIABLE;
	}

	packet = enet_packet_create(nullptr, 0, pflags);
}

//-----------------------------------//

static size_t GetCompressionBufferSize(size_t size)
{
	// From the FastLZ docs: "The output buffer must be at least 5%
	// larger than the input buffer and can not be smaller than 66 bytes."

	size_t bufSize = size + size_t(size * 0.05f);
	bufSize = std::max(bufSize, (size_t)128);

	return bufSize;
}

//-----------------------------------//

void Message::prepare()
{
	if( !packet ) createPacket();

	size_t totalSize = (size_t) ms->position;

	size_t bufSize = GetCompressionBufferSize(totalSize);
	enet_packet_resize(packet, bufSize);

	MemoryStream pms;
	StreamMemoryInit(&pms);
	StreamMemorySetRawBuffer(&pms, packet->data);

	EncodeVariableInteger(&pms, id);
	StreamWrite(&pms, &flags, sizeof(flags));
	
	if( GetBitFlag(flags, MessageFlags::Compressed) )
	{
		uint8* in = &ms->data[0];
		uint8* out = pms.buffer + pms.position;
		int32 length = fastlz_compress_level(1, in, totalSize, out);
		pms.position += length;
	}
	else
	{
		enet_packet_resize(packet, totalSize + pms.position);
		StreamWrite(&pms, &ms->data[0], totalSize);
	}

	packet->dataLength = (size_t) pms.position;
}

//-----------------------------------//

void Message::setPacket(ENetPacket* packet)
{
	MemoryStream pms;
	StreamMemoryInit(&pms);
	StreamMemorySetRawBuffer(&pms, packet->data);

	uint64 id;

	if( !DecodeVariableInteger(&pms, id) )
		return;

	this->id = (MessageId) id;
	this->packet = packet;

	StreamReadBuffer(&pms, &flags, sizeof(MessageFlags::Enum));

	uint8* in = packet->data + pms.position;
	size_t inSize = packet->dataLength - (size_t) pms.position;

	if( GetBitFlag(flags, MessageFlags::Compressed) )
	{
		size_t bufSize = GetCompressionBufferSize(packet->dataLength);
		StreamResize(ms, bufSize);

		int32 length = fastlz_decompress(in, inSize, &ms->data[0], ms->data.size());
		StreamResize(ms, length);
	}
	else
	{
		StreamResize(ms, inSize);
		StreamWrite(ms, in, inSize);
	}

	freePacket = true;
}

//-----------------------------------//

void Message::write(const Object* object)
{
	SetBitFlag(flags, MessageFlags::Binary, true);

	Serializer* serializer = SerializerCreateBinary(AllocatorGetThis(), 0);
	serializer->stream = ms;

	SerializerSave(serializer, object);
	
	Deallocate(serializer);
}

//-----------------------------------//

MessagePtr MessageCreate(MessageId id)
{
	MessagePtr message = Allocate(AllocatorGetNetwork(), Message, id);
	return message;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif