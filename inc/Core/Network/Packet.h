/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/API.h"

#include "Core/References.h"
#include "Core/Stream.h"
#include "Core/Serialization.h"

struct _ENetPacket;
typedef _ENetPacket ENetPacket;

FWD_DECL_INTRUSIVE(Session)
FWD_DECL_INTRUSIVE(Packet)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef uint16 PacketId;

enum class FLD_FLAGS PacketFlags : uint8
{
	Binary		= 1 << 0,
	Encrypted	= 1 << 1,
	Compressed	= 1 << 2,
	Reliable	= 1 << 3,
};

class API_CORE Packet : public ReferenceCounted
{
public:

	Packet(PacketId id);
	~Packet();

	// Writes an object to the packet.
	void FLD_IGNORE write(const Object* object);

	// Accesses the packet flags.
	ACCESSOR(Flags, PacketFlags, flags)

	// Accesses the packet id.
	GETTER(Id, PacketId, id)

	// Gets the internal packet.
	GETTER(Packet, ENetPacket* FLD_IGNORE, packet)

	// Creates a network packet.
	void FLD_IGNORE createPacket();

	// Sets the internal packet.
	void FLD_IGNORE setPacket(ENetPacket* packet);

	// Prepares the packet for sending.
	void FLD_IGNORE prepare();

	// Writes a POD type to the packet.
	template<typename T> void FLD_IGNORE writeRaw(const T& pod)
	{
		ms->data.resize(ms->data.size() + sizeof(T));
		memcpy(&ms->data[0] + ms->position, &pod, sizeof(T));
		ms->position += sizeof(T);
	}

	// Reads a POD type from the packet.
	template<typename T> T* FLD_IGNORE readRaw()
	{
		T* pod = (T*)(&ms->data[0] + ms->position);
		ms->position += sizeof(T);
		return pod;
	}

	void write(std::vector<byte>& data);
	std::vector<byte> read();

private:

	// Packet id.
	PacketId id;

	// Packet flags.
	PacketFlags flags;

	// Packet data.
	MemoryStream* ms;

	// Network packet;
	ENetPacket* packet;

	// Keeps if packet needs to be freed.
	bool freePacket;
};

// Creates a new packet using the network allocator.
API_CORE PacketPtr PacketCreate(PacketId id);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Packet)

//-----------------------------------//

NAMESPACE_CORE_END