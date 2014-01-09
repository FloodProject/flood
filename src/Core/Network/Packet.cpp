/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Network/Packet.h"
#include "Core/Network/Network.h"
#include "Core/Stream.h"
#include "Core/SerializationHelpers.h"
#include "Core/Utilities.h"
#include <cmath>

#if defined(PLATFORM_WINDOWS) && !defined(WIN32)
#define WIN32
#endif

#define NOMINMAX
#include <enet/enet.h>

#include <fastlz.h>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

const size_t DefaultPacketSize = 512;

Packet::Packet(PacketId id)
	: id(id)
	, ms(DefaultPacketSize)
	, packet(nullptr)
	, freePacket(false)
	, flags((PacketFlags)0)
{
	SetBitFlag(flags, PacketFlags::Reliable, true);
}

//-----------------------------------//

Packet::~Packet()
{
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

	size_t totalSize = (size_t) ms.getPosition();

	size_t bufSize = GetCompressionBufferSize(totalSize);
	enet_packet_resize(packet, bufSize);

	MemoryStream pms;
	pms.setRawBuffer(packet->data);

	EncodeVariableInteger(&pms, id);
	pms.write((uint8*) &flags, sizeof(flags));
	
	if( GetBitFlag(flags, PacketFlags::Compressed) )
	{
		uint8* in = ms.data.Buffer();
		uint8* out = pms.buffer + pms.position;
		int32 length = fastlz_compress_level(1, in, totalSize, out);
		pms.position += length;
	}
	else
	{
		enet_packet_resize(packet, totalSize + pms.position);
		pms.write(ms.data.Buffer(), totalSize);
	}

	packet->dataLength = (size_t) pms.position;
}

//-----------------------------------//

void Packet::setPacket(ENetPacket* packet)
{
	MemoryStream pms;
	pms.setRawBuffer(packet->data);

	uint64 id;

	if( !DecodeVariableInteger(&pms, id) )
		return;

	this->id = (PacketId) id;
	this->packet = packet;

	pms.readBuffer(&flags, sizeof(PacketFlags));

	uint8* in = packet->data + pms.position;
	size_t inSize = packet->dataLength - (size_t) pms.position;

	if( GetBitFlag(flags, PacketFlags::Compressed) )
	{
		size_t bufSize = GetCompressionBufferSize(packet->dataLength);
		ms.resize(bufSize);

		int32 length = fastlz_decompress(in, inSize, ms.data.Buffer(), ms.data.Size());
		ms.resize(length);
	}
	else
	{
		ms.resize(inSize);
		ms.write(in, inSize);
	}

	freePacket = true;
}

//-----------------------------------//

void Packet::write(const Object* object)
{
	SetBitFlag(flags, PacketFlags::Binary, true);

	SerializerBinary serializer(AllocatorGetThis(), 0);
	serializer.stream = &ms;

	serializer.save(object);
}

//-----------------------------------//

int Packet::size() const
{
	return ms.getPosition();
}

//-----------------------------------//

void Packet::clear()
{
	ms.setPosition(0, StreamSeekMode::Absolute);
}

//-----------------------------------//

void Packet::write(byte* data, int size)
{
	int streamSize = ms.size();
	int streamPosition = ms.getPosition();
	if(streamPosition+size > streamSize)
		ms.resize(2*streamSize+size);

	ms.write(data,size);
}

void Packet::write(Vector<byte>& data)
{
	write(data.Buffer(), data.Size());
}

Vector<byte> Packet::read() const
{
	Vector<uint8> vec(ms.data.Buffer(), size());
	return vec;
}

//-----------------------------------//

PacketPtr PacketCreate(PacketId id)
{
	PacketPtr packet = Allocate(AllocatorGetNetwork(), Packet, id);
	return packet;
}

//-----------------------------------//

NAMESPACE_CORE_END
