/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/GeometryBuffer.h"
#include "Core/Log.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

VertexElement::VertexElement()
	: stride(-1)
	, offset(0)
	, size(0)
{
}

VertexElement::VertexElement(VertexAttribute attribute,
	VertexDataType type, uint8 components)
	: stride(-1)
	, offset(0)
	, size(0)
{
	this->attribute = attribute;
	this->type = type;
	this->components = components;
}

//-----------------------------------//

static int GetVertexDataTypeSize(VertexDataType type)
{
	switch(type)
	{
	case VertexDataType::Float:   return sizeof(float);
	case VertexDataType::Integer: return sizeof(int);
	case VertexDataType::Byte:    return sizeof(uint8);
	}

	LogAssert("Unknown vertex type");

	return 0;
}

//-----------------------------------//

uint8 VertexElement::getSize() const
{
	return components * GetVertexDataTypeSize(type);
}

//-----------------------------------//

void VertexDeclaration::add(const VertexElement& elem)
{
	decls.Push(elem);
}

//-----------------------------------//

void VertexDeclaration::add(const VertexElementP& pod)
{
	VertexElement elem(pod.attribute, pod.type, pod.components);
	decls.Push(elem);
}

//-----------------------------------//

void VertexDeclaration::add(VertexAttribute attribute, int numComponents)
{
	VertexElement elem(attribute, VertexDataType::Float, numComponents);
	decls.Push(elem);
}

//-----------------------------------//

VertexElement* VertexDeclaration::find(VertexAttribute attribute) const
{
	for(size_t i = 0; i < decls.Size(); i++)
	{
		const VertexElement& elem = decls[i];
		if(elem.attribute != attribute) continue;
		
		return (VertexElement*) &elem;
	}
	
	return nullptr;
}

//-----------------------------------//

void VertexDeclaration::reset()
{
	decls.Clear();
}

//-----------------------------------//

uint8 VertexDeclaration::getVertexSize() const
{
	uint8 totalSize = 0;

	for(size_t i = 0; i < decls.Size(); i++)
	{
		const VertexElement& elem = decls[i];
		totalSize += elem.getSize();
	}

	return totalSize;
}

//-----------------------------------//

uint8 VertexDeclaration::getOffset(VertexAttribute attribute) const
{
	uint8 totalOffset = 0;

	for(size_t i = 0; i < decls.Size(); i++)
	{
		const VertexElement& elem = decls[i];

		if(elem.attribute == attribute) return totalOffset;

		// If there is no stride, the elements are tightly packed.
		if(elem.stride == 0) continue;

		totalOffset += elem.getSize();
	}

	return totalOffset;
}

//-----------------------------------//

void VertexDeclaration::calculateStrides()
{
	uint8 vertexSize = getVertexSize();

	for(size_t i = 0; i < decls.Size(); ++i)
	{
		VertexElement& elem = decls[i];
		elem.stride = vertexSize;
		elem.offset = getOffset(elem.attribute);
	}
}

//-----------------------------------//

GeometryBuffer::GeometryBuffer()
	: usage(BufferUsage::Static)
	, access(BufferAccess::Read)
	, indexSize(16)
	, needsRebuild(false)
	, hash(0)
{
}

//-----------------------------------//

GeometryBuffer::GeometryBuffer(BufferUsage usage, BufferAccess access)
{
}

//-----------------------------------//

GeometryBuffer::~GeometryBuffer()
{
}

//-----------------------------------//

void GeometryBuffer::clear()
{
	data.Clear();
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::set(VertexAttribute attribute, uint8* buf, uint32 size)
{
	VertexElement* elem = declarations.find(attribute);
	if( !elem ) return;

	if(elem->size != size)
	{
		LogAssert("Vertex element sizes do not match");
		return;
	}

	std::copy(buf, buf+size, data.begin() + elem->offset);
}

//-----------------------------------//

void GeometryBuffer::set(uint8* buf, uint32 size)
{
	clear();
	add(buf, size);
}

//-----------------------------------//

void GeometryBuffer::add(uint8* buf, uint32 size)
{
	data.Resize(data.Size() + size);
	std::copy(buf, buf+size, data.end() - size);
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::setIndex(uint8* buf, uint32 size)
{
	indexData.Clear();
	addIndex(buf, size);
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::addIndex(uint8* buf, uint32 size)
{
	indexData.Resize(indexData.Size() + size);
	std::copy(buf, buf+size, indexData.end() - size);
	forceRebuild();
}

//-----------------------------------//

void GeometryBuffer::addIndex(uint16 index)
{
	return addIndex((uint8*) &index, sizeof(uint16));
}

//-----------------------------------//

bool GeometryBuffer::isIndexed() const
{
	return !indexData.Empty();
}

//-----------------------------------//

uint32 GeometryBuffer::getNumVertices() const
{
	if( declarations.decls.Empty() ) return 0;

	uint8 sizeVertex = declarations.getVertexSize();
	
	// Assume all separate data is the same size.
	if( sizeVertex == 0 )
	{
		const VertexElement& elem = declarations.decls[0];
		return elem.size / elem.getSize();
	}

	return data.Size() / sizeVertex;
}

//-----------------------------------//

uint32 GeometryBuffer::getNumIndices() const
{
	uint8 indexSizeBytes = indexSize / 8;
	assert( indexSizeBytes != 0 );

	uint32 numIndices = indexData.Size() / indexSizeBytes;
	return numIndices;
}

void GeometryBuffer::clearIndexes()
{
    indexData.Clear();
}

//-----------------------------------//

float* GeometryBuffer::getAttribute(VertexAttribute attribute, uint32 i) const
{
	VertexElement* elem = declarations.find(attribute);
	if( !elem ) return nullptr;

	if(elem->stride == 0)
	{
		uint8 elemSize = elem->getSize();

		const uint8* loc = &data.Front() + elem->offset + (elemSize * i);
		return (float*) loc;
	}

	// Calculate the starting position of the attribute data.
	uint8 totalSize = declarations.getVertexSize();
	uint8 localOffset = declarations.getOffset(elem->attribute);

	// TODO: Check the bounds and return null in that case too.

	const uint8* loc = &data.Front() + (totalSize * i) + localOffset;
	return (float*) loc;
}

//-----------------------------------//

int8 GeometryBuffer::getAttributeStride(VertexAttribute attribute) const
{
	VertexElement* elem = declarations.find(attribute);
	if( !elem ) return -1;

	return elem->stride;
}

//-----------------------------------//

void GeometryBuffer::forceRebuild()
{
	needsRebuild = true;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END
