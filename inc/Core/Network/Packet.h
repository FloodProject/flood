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
	Signed		= 1 << 4,
};

class API_CORE Packet : public ReferenceCounted
{
public:

	Packet(PacketId id);
	~Packet();

	// Writes an object to the packet.
	FLD_IGNORE void write(const Object* object);

	// Accesses the packet flags.
	ACCESSOR(Flags, PacketFlags, flags)

	// Accesses the packet processed flags.
	ACCESSOR(ProcessedFlags, PacketFlags, processedFlags)

	// Accesses the packet id.
	GETTER(Id, PacketId, id)

	// Gets the internal packet.
	FLD_IGNORE GETTER(Packet, ENetPacket*, packet)

	// Gets the packet memory stream.
	FLD_IGNORE GETTER(MemoryStream, MemoryStream*, (MemoryStream*)&ms)

	// Creates a network packet.
	FLD_IGNORE void createPacket();

	// Sets the internal packet.
	FLD_IGNORE void setPacket(ENetPacket* packet);

	// Prepares the packet for sending.
	FLD_IGNORE void prepare();

	// Writes a POD type to the packet.
	FLD_IGNORE template<typename T> void writeRaw(const T& pod)
	{
		ms->data.resize(ms->data.size() + sizeof(T));
		memcpy(&ms->data[0] + ms->position, &pod, sizeof(T));
		ms->position += sizeof(T);
	}

	// Reads a POD type from the packet.
	FLD_IGNORE template<typename T> T* readRaw()
	{
		T* pod = (T*)(&ms->data[0] + ms->position);
		ms->position += sizeof(T);
		return pod;
	}

	// Returns the size of the packet's data.
	int size() const;

	// Clears all the packet's data.
	void clear();

	// Appends new data to the packet.
	FLD_IGNORE void write(byte* data, int size);

	// Appends new data to the packet.
	void write(Array<byte>& data);

	// Returns a copy of the packet's data
	Array<byte> read() const;

private:

	// Packet id.
	PacketId id;

	// Packet flags.
	PacketFlags flags;

	// Packet processed flags.
	PacketFlags processedFlags;

	// Packet data.
	MemoryStream ms;

	// Network packet;
	ENetPacket* packet;

	// Keeps if packet needs to be freed.
	bool freePacket;
};

// Creates a new packet using the network allocator.
FLD_IGNORE API_CORE PacketPtr PacketCreate(PacketId id);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Packet)

//-----------------------------------//

NAMESPACE_CORE_END