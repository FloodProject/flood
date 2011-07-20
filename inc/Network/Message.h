/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"

struct _ENetPacket;
typedef _ENetPacket ENetPacket;

FWD_DECL_INTRUSIVE(Session)
FWD_DECL_INTRUSIVE(Message)

NAMESPACE_CORE_BEGIN

//-----------------------------------//

typedef uint32 MessageId;

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

	// Accesses the message type.
	ACESSOR(Type, MessageType::Enum, type)

	// Accesses the message id.
	ACESSOR(Id, MessageId, id)

	// Accesses the message data.
	ACESSOR(Data, const MessageData&, data)

	// Gets the internal packet.
	ACESSOR(Packet, ENetPacket*, packet)

	// Prepares the message for sending.
	void prepare();

protected:
	
	// Creates a network packet.
	void createPacket();

	// Message id.
	MessageId id;

	// Message type.
	MessageType::Enum type;

	// Message data.
	MessageData data;

	// Network packet;
	ENetPacket* packet;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Message)

//-----------------------------------//

NAMESPACE_CORE_END