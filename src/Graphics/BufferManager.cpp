/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/BufferManager.h"
#include "Graphics/RenderBackend.h"
#include "Graphics/GeometryBuffer.h"

#include "Core/Containers/Hash.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

BufferEntry::BufferEntry()
	: vb(nullptr)
	, ib(nullptr)
{
}

//-----------------------------------//

BufferManager::BufferManager()
	: backend(nullptr)
	, buffers(*AllocatorGetHeap())
{
}

//-----------------------------------//

BufferManager::~BufferManager()
{
}

//-----------------------------------//

BufferEntry* BufferManager::getBuffer(const GeometryBuffer* gb)
{
	if( !gb ) return nullptr;

	if(hash::has(buffers, (uint64)gb))
		return const_cast<BufferEntry*>(&(hash::get(buffers, (uint64)gb, BufferEntry())));

	BufferEntry entry;
	entry.vb = backend->createVertexBuffer();
	entry.vb->setBufferAccess( gb->getBufferAccess() );
	entry.vb->setBufferUsage( gb->getBufferUsage() );
	entry.vb->setGeometryBuffer( gb );

	if( gb->isIndexed() )
	{
		entry.ib = backend->createIndexBuffer();
		entry.ib->setBufferAccess( gb->getBufferAccess() );
		entry.ib->setBufferUsage( gb->getBufferUsage() );
		entry.ib->setGeometryBuffer( gb );
	}

	hash::set(buffers, (uint64)gb, entry);
	return const_cast<BufferEntry*>(&(hash::get(buffers, (uint64)gb, BufferEntry())));
}

//-----------------------------------//

VertexBufferPtr BufferManager::getVertexBuffer(const GeometryBuffer* gb)
{
	BufferEntry* bufs = getBuffer(gb);
	if (!bufs ) return nullptr;

	return bufs->vb;
}

//-----------------------------------//

IndexBufferPtr BufferManager::getIndexBuffer(const GeometryBuffer* gb)
{
	BufferEntry* bufs = getBuffer(gb);
	if (!bufs ) return nullptr;

	return bufs->ib;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END