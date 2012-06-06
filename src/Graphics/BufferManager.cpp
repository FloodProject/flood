/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/BufferManager.h"
#include "Graphics/RenderBackend.h"

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

	auto it = buffers.find(gb);

	if( it != buffers.end() )
	{
		// Buffers already allocated.
		return &it->second;
	}

	BufferEntry& entry = buffers[gb];
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

	return &entry;
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