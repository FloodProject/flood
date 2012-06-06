/************************************************************************
*
* vaporEngine by triton © (2008-2010)
*
*	<http://www.vapor3d.org>
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

Buffer::Buffer(BufferUsage::Enum usage, BufferAccess::Enum access) 
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