/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/Message.h"

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

}

//-----------------------------------//

void Message::init()
{

}

//-----------------------------------//

void Message::initData(const std::vector<byte>& buf)
{

}

//-----------------------------------//

NAMESPACE_END