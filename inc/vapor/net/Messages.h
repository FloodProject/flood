/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

namespace MessageType
{
	enum Enum
	{
		ResourceRequest = 0,
		ResourceResponse,
		ResourceIndexRequest,
		ResourceLockRequest,
		ResourceUnlockRequest
	};
}

//-----------------------------------//

} // end namespace