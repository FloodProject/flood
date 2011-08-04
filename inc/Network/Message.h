/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"

struct _ENetPacket;
typedef _ENetPacket ENetPacket;

FWD_DECL_INTRUSIVE(Session)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef uint16 MessageId;

namespace MessageType
{
	enum Enum
	{
		Reliable,
		Unreliable
	};
}

typedef std::vector<byte> MessageData;

class Message : public ReferenceCounted
{
public:

	Message();
	~Message();

	// Creates a network packet.
	void createPacket();

	// Accesses the message type.
	ACESSOR(Type, MessageType::Enum, type)

	// Accesses the message id.
	GETTER(Id, MessageId, id)

	// Accesses the message data.
	ACESSOR(Data, const MessageData&, data)

	// Gets the internal packet.
	GETTER(Packet, ENetPacket*, packet)

	// Sets the internal packet.
	void setPacket(ENetPacket* packet);

	// Prepares the message for sending.
	void prepare();

	// Writes a POD type to the message.
	template<typename T> void write(const T& pod)
	{
		data.resize(data.size() + sizeof(T));
		memcpy(&data.front() + index, &pod, sizeof(T));
		index += sizeof(T);
	}

	// Reads a POD type from the message.
	template<typename T> T* read()
	{
		T* pod = (T*)(&data.front() + index);
		index += sizeof(T);
		return pod;
	}

	// Message id.
	MessageId id;

	// Message type.
	MessageType::Enum type;

	// Message data.
	MessageData data;

	// Current index in data.
	int index;

	// Network packet;
	ENetPacket* packet;
};

// Creates a new message using the network allocator.
API_CORE MessagePtr MessageCreate(MessageId id);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Message)

//-----------------------------------//

NAMESPACE_CORE_END