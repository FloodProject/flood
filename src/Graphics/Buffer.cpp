/************************************************************************
*
* vaporEngine by triton © (2008-2010)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/Buffer.h"

NAMESPACE_GRAPHICS_BEGIN
		
//-----------------------------------//

Buffer::Buffer() 
	: id(0)
	, usage(BufferUsage::Static)
	, access(BufferAccess::Read)
	, gb(nullptr)
{
}

//-----------------------------------//

Buffer::Buffer(BufferUsage usage, BufferAccess access) 
	: usage(usage)
	, access(access)
{
}

//-----------------------------------//

Buffer::~Buffer()
{

}

//-----------------------------------//

NAMESPACE_GRAPHICS_END