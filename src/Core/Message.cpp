/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "net/Message.h"

#ifdef ENABLE_NETWORKING_ZMQ

NAMESPACE_BEGIN

//-----------------------------------//

Message::Message()
{ }

//-----------------------------------//

Message::Message(const std::vector<byte>& buf)
{
	initData(buf);
}

//-----------------------------------//

Message::~Message()
{
	int ret = zmq_msg_close(&message);
	assert(ret == 0);
}

//-----------------------------------//

void Message::init()
{
	int ret = zmq_msg_init(&message);
	assert(ret == 0);
}

//-----------------------------------//

void Message::initData(const std::vector<byte>& buf)
{
	if( buf.empty() )
		return;

	data = buf;

	int ret = zmq_msg_init_data(&message,
		&data.front(), data.size(), nullptr, nullptr);
	assert(ret == 0);
}

//-----------------------------------//

NAMESPACE_END

#endif