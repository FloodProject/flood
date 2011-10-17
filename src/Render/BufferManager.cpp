/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/BufferManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

BufferEntry::BufferEntry()
	: vb(nullptr)
	, ib(nullptr)
{
}

//-----------------------------------//

BufferManager::BufferManager()
{
}

//-----------------------------------//

BufferManager::~BufferManager()
{
}

//-----------------------------------//

BufferEntry* BufferManager::getBuffer(const GeometryBufferPtr& gb)
{
	if( !gb ) return nullptr;

	auto it = buffers.find(gb);

	if( it != buffers.end() )
	{
		// Buffers already allocated.
		return &it->second;
	}

	BufferEntry& entry = buffers[gb];
	entry.vb = AllocateThis(VertexBuffer);
	entry.vb->setBufferAccess( gb->getBufferAccess() );
	entry.vb->setBufferUsage( gb->getBufferUsage() );
	entry.vb->setGeometryBuffer( gb.get() );

	if( gb->isIndexed() )
	{
		entry.ib = AllocateThis(IndexBuffer);
		entry.ib->setBufferAccess( gb->getBufferAccess() );
		entry.ib->setBufferUsage( gb->getBufferUsage() );
	}

	return &entry;
}

//-----------------------------------//

VertexBufferPtr BufferManager::getVertexBuffer(const GeometryBufferPtr& gb)
{
	BufferEntry* bufs = getBuffer(gb);
	if (!bufs ) return nullptr;

	return bufs->vb;
}

//-----------------------------------//


IndexBufferPtr BufferManager::getIndexBuffer(const GeometryBufferPtr& gb)
{
	BufferEntry* bufs = getBuffer(gb);
	if (!bufs ) return nullptr;

	return bufs->ib;
}

//-----------------------------------//

NAMESPACE_ENGINE_END