/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/Stream.h"

struct _ENetPacket;
typedef _ENetPacket ENetPacket;

FWD_DECL_INTRUSIVE(Session)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef uint16 MessageId;

namespace MessageFlags
{
	enum Enum : uint8
	{
		Binary		= 1 << 0,
		Encrypted	= 1 << 1,
		Compressed	= 1 << 2,
		Reliable	= 1 << 3,
	};
}

class Message : public ReferenceCounted
{
public:

	Message(MessageId id);
	~Message();

	// Creates a network packet.
	void createPacket();

	// Accesses the message flags.
	ACESSOR(Flags, uint8, flags)

	// Accesses the message id.
	GETTER(Id, MessageId, id)

	// Gets the internal packet.
	GETTER(Packet, ENetPacket*, packet)

	// Sets the internal packet.
	void setPacket(ENetPacket* packet);

	// Prepares the message for sending.
	void prepare();

	// Message id.
	MessageId id;

	// Message flags.
	uint8 flags;

	// Message data.
	MemoryStream* ms;

	// Network packet;
	ENetPacket* packet;

	// Writes a POD type to the message.
	template<typename T> void write(const T& pod)
	{
		data.resize(data.size() + sizeof(T));
		memcpy(&ms->data[0] + ms->position, &pod, sizeof(T));
		ms->position += sizeof(T);
	}

	// Reads a POD type from the message.
	template<typename T> T* read()
	{
		T* pod = (T*)(&ms->data[0] + ms->position);
		ms->position += sizeof(T);
		return pod;
	}
};

// Creates a new message using the network allocator.
API_CORE MessagePtr MessageCreate(MessageId id);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Message)

//-----------------------------------//

NAMESPACE_CORE_END