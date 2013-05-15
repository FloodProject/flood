/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Stream.h"
#include "Core/SerializationHelpers.h"

#include "Core/Network/Packet.h"
#include "Core/Network/Session.h"
#include "Core/Network/Peer.h"
#include "Core/Network/Network.h"

#define NOMINMAX
#include <enet/enet.h>

#include <fastlz.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Packet::Packet(PacketId id)
	: id(id)
	, ms(nullptr)
	, packet(nullptr)
	, freePacket(false)
	, flags((PacketFlags)0)
{
	ms = StreamCreateFromMemory(AllocatorGetThis(), 512);
	SetBitFlag(flags, PacketFlags::Reliable, true);
}

//-----------------------------------//

Packet::~Packet()
{
	Deallocate(ms);

	if(freePacket)
	{
		enet_packet_destroy(packet);
		packet = nullptr;
	}
}

//-----------------------------------//

void Packet::createPacket()
{
	void* data = nullptr;
	size_t size = 0;
	uint32 pflags = 0;

	if(GetBitFlag(flags, PacketFlags::Reliable))
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

void Packet::prepare()
{
	if( !packet ) createPacket();

	size_t totalSize = (size_t) ms->position;

	size_t bufSize = GetCompressionBufferSize(totalSize);
	enet_packet_resize(packet, bufSize);

	MemoryStream pms;
	StreamMemoryInit(&pms);
	StreamMemorySetRawBuffer(&pms, packet->data);

	EncodeVariableInteger(&pms, id);
	StreamWrite(&pms, (uint8*) &flags, sizeof(flags));
	
	if( GetBitFlag(flags, PacketFlags::Compressed) )
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

void Packet::setPacket(ENetPacket* packet)
{
	MemoryStream pms;
	StreamMemoryInit(&pms);
	StreamMemorySetRawBuffer(&pms, packet->data);

	uint64 id;

	if( !DecodeVariableInteger(&pms, id) )
		return;

	this->id = (PacketId) id;
	this->packet = packet;

	StreamReadBuffer(&pms, &flags, sizeof(PacketFlags));

	uint8* in = packet->data + pms.position;
	size_t inSize = packet->dataLength - (size_t) pms.position;

	if( GetBitFlag(flags, PacketFlags::Compressed) )
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

void Packet::write(const Object* object)
{
	SetBitFlag(flags, PacketFlags::Binary, true);

	Serializer* serializer = SerializerCreateBinary(AllocatorGetThis(), 0);
	serializer->stream = ms;

	SerializerSave(serializer, object);
	
	Deallocate(serializer);
}

//-----------------------------------//

void Packet::write(std::vector<byte> data)
{
	StreamSetPosition(ms, 0, StreamSeekMode::Absolute);
	StreamResize(ms, data.size());
	StreamWrite(ms,data.data(),data.size());
}

//-----------------------------------//

PacketPtr PacketCreate(PacketId id)
{
	PacketPtr packet = Allocate(AllocatorGetNetwork(), Packet, id);
	return packet;
}

//-----------------------------------//

NAMESPACE_CORE_END

#endif