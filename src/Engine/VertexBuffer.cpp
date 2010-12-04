/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/VertexBuffer.h"
#include "vapor/render/GL.h"

namespace vapor {

//-----------------------------------//

VertexBuffer::VertexBuffer(BufferUsage::Enum usage, BufferAccess::Enum access)
	: Buffer(usage, access)
	, built(false)
	, numVertices(0)
{ }

//-----------------------------------//

VertexBuffer::~VertexBuffer()
{
	clear();
}

//-----------------------------------//

bool VertexBuffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, id );

	if( glHasError("Error binding vertex buffer") )
		return false;

	bindPointers();

	return true;
}

//-----------------------------------//

bool VertexBuffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	if( glHasError("Error unbinding vertex buffer") )
		return false;

	if( !built )
		return true;

	AttributeMap::const_iterator it;
	for( it = attributes.cbegin(); it != attributes.cend(); it++ )
	{
		glDisableVertexAttribArray( it->first );
	}

	return true;
}

//-----------------------------------//

void VertexBuffer::bindPointers()
{
	if( !built )
		return;

	int offset = 0;

	AttributeMap::const_iterator it;
	for( it = attributes.cbegin(); it != attributes.cend(); it++ )
	{
		const Attribute& attr = it->second;

		glEnableVertexAttribArray( it->first );

		if( glHasError("Error enabling vertex attribute array") )
			return;

		glVertexAttribPointer( it->first, attr.components,
			attr.type, GL_FALSE, 0, (void*) offset );

		if( glHasError("Error binding pointers to buffer") )
			return;

		offset += attr.data.size();
	}
}

//-----------------------------------//

bool VertexBuffer::isValid() const
{
	// Check that we have a valid buffer.
	if( !glIsBuffer(id) )
	{
		Log::warn( "Vertex buffer is not valid" );
		return false;
	}

	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum num, 
					    const std::vector<Vector3>& data )
{
	built = false;

	Attribute attr;
	attr.components = 3;
	attr.size = sizeof(float);
	attr.type = GL_FLOAT;
	attr.data.resize( data.size() * sizeof(Vector3) );
	
	if( data.size() != 0)
		memcpy( &attr.data[0], &data[0], attr.data.size() );
	
	attributes[num] = attr;
	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum num,
					    const std::vector<float>& data )
{
	built = false;

	Attribute attr;
	attr.components = 1;
	attr.size = sizeof(float);
	attr.type = GL_FLOAT;
	attr.data.resize( data.size() * sizeof(int) );
	
	if( data.size() != 0)
		memcpy( &attr.data[0], &data[0], attr.data.size() );
	
	attributes[num] = attr;
	return true;
}

//-----------------------------------//

bool VertexBuffer::build()
{
	bind();

	// Check that all vertex attributes elements are the same size.
	//if( !checkSize() )
	//{
	//	Log::warn( "Vertex buffers elements have different sizes" );
	//	return false;
	//}

	// Reserve space for all the vertex elements.
	glBufferData( GL_ARRAY_BUFFER, getSize(), nullptr, getGLBufferType() );

	if( glHasError("Could not allocate storage for buffer") )
		return false;

	int offset = 0;
	
	AttributeMap::const_iterator it;
	for( it = attributes.cbegin(); it != attributes.cend(); it++ )
	{
		const std::vector<byte>& vec = it->second.data;

		if( vec.empty() )
			continue;

		glBufferSubData( GL_ARRAY_BUFFER, offset, vec.size(), &vec[0] );

		if( glHasError("Could not buffer the data") )
			return false;

		offset += vec.size();
	}

	built = true;
	return true;
}

//-----------------------------------//

bool VertexBuffer::checkSize() const
{
	if( attributes.empty() )
		return false;

	int initialSize = 0;

	AttributeMap::const_iterator it;
	for( it = attributes.cbegin(); it != attributes.cend(); it++ )
	{
		const Attribute& attr = it->second;
		int size = attr.data.size();

		if( size == 0 )
			return false;

		if( !initialSize )
		{
			initialSize = size;
			numVertices = size / (attr.components * attr.size);
		}

		if(size != initialSize)
			return false;
	}

	return true;
}

//-----------------------------------//

uint VertexBuffer::getSize() const
{
	uint totalBytes = 0;

	AttributeMap::const_iterator it;
	for( it = attributes.cbegin(); it != attributes.cend(); it++ )
	{
		totalBytes += it->second.data.size();
	}

	return totalBytes;
}

//-----------------------------------//

std::vector<Vector3>& VertexBuffer::getVertices() const
{
	return getAttribute( VertexAttribute::Position );
}

//-----------------------------------//

std::vector<Vector3>&
VertexBuffer::getAttribute( VertexAttribute::Enum attr ) const
{
	AttributeMap::iterator it;
	it = attributes.find(attr);

	if( it == attributes.end() )
		assert( 0 && "Can't return null reference" );
	
	Attribute& p = (*it).second;
	std::vector<byte>& arr = p.data;

	return (std::vector<Vector3>&)(arr);
}

//-----------------------------------//

void VertexBuffer::forceRebuild()
{
	built = false;
}

//-----------------------------------//

uint VertexBuffer::getNumAttributes() const
{
	return attributes.size();
}

//-----------------------------------//

uint VertexBuffer::getNumVertices() const
{
	checkSize();
	return numVertices;
}

//-----------------------------------//

void VertexBuffer::clear()
{
	attributes.clear();
}

//-----------------------------------//

bool VertexBuffer::isBuilt() const
{
	return built;
}

//-----------------------------------//

} // end namespace

#endif