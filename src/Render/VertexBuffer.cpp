/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "Render/VertexBuffer.h"
#include "Render/GL.h"

NAMESPACE_ENGINE_BEGIN

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
	return !glHasError("Error binding vertex buffer");
}

//-----------------------------------//

bool VertexBuffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	return !glHasError("Error unbinding vertex buffer");
}

//-----------------------------------//

#define IfAttributeExists(attr)										\
	it = attributes.find(VertexAttribute::attr);					\
	if(it != attributes.end())

#define IfAttributeIs(attr)											\
	if(index == VertexAttribute::attr)

#define EnableArrayPointer(arr, func)								\
	{ glEnableClientState(arr);										\
	func(attr.type, 0, (void*) offset); }

#define EnableArrayPointerComponents(arr, func)						\
	{ glEnableClientState(arr);										\
	func(attr.components, attr.type, 0, (void*) offset); }

#define EnableTexArrayPointer(i)									\
	{ glClientActiveTexture(GL_TEXTURE0+i);							\
	EnableArrayPointerComponents(GL_TEXTURE_COORD_ARRAY, glTexCoordPointer) }

void VertexBuffer::bindPointers()
{
	if( !built ) return;

	int offset = 0;

	AttributeMap::const_iterator it;
	for( it = attributes.begin(); it != attributes.end(); it++ )
	{
		int index = it->first;
		const Attribute& attr = it->second;
		
		IfAttributeIs(Position) EnableArrayPointerComponents(GL_VERTEX_ARRAY, glVertexPointer)
		IfAttributeIs(Normal) EnableArrayPointer(GL_NORMAL_ARRAY, glNormalPointer);
		IfAttributeIs(Color) EnableArrayPointerComponents(GL_COLOR_ARRAY, glColorPointer)
		IfAttributeIs(TexCoord0) EnableTexArrayPointer(0)
		IfAttributeIs(TexCoord1) EnableTexArrayPointer(1)
		IfAttributeIs(TexCoord2) EnableTexArrayPointer(2)
		IfAttributeIs(TexCoord3) EnableTexArrayPointer(3)

		offset += attr.data.size();
	}
}

//-----------------------------------//

#define DisableTexCoordArray(i)										\
	{ glClientActiveTexture(GL_TEXTURE0+1);							\
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); }

void VertexBuffer::unbindPointers()	
{
	if( !built ) return;

	AttributeMap::const_iterator it;

	IfAttributeExists(Position) glDisableClientState(GL_VERTEX_ARRAY);
	IfAttributeExists(Normal) glDisableClientState(GL_NORMAL_ARRAY);
	IfAttributeExists(Color) glDisableClientState(GL_COLOR_ARRAY);
	//IfAttributeExists(SecondaryColor) glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
	IfAttributeExists(FogCoord) glDisableClientState(GL_FOG_COORD_ARRAY);
	IfAttributeExists(TexCoord0) DisableTexCoordArray(0)
	IfAttributeExists(TexCoord1) DisableTexCoordArray(1)
	IfAttributeExists(TexCoord2) DisableTexCoordArray(2)
	IfAttributeExists(TexCoord3) DisableTexCoordArray(3)
}

//-----------------------------------//

void VertexBuffer::bindGenericPointers()
{
	if( !built ) return;

	int offset = 0;

	AttributeMap::const_iterator it;
	for( it = attributes.begin(); it != attributes.end(); it++ )
	{
		VertexAttribute::Enum index = it->first;
		const Attribute& attr = it->second;

		glEnableVertexAttribArray(index);

		if( glHasError("Error enabling vertex attribute array") )
			return;

		glVertexAttribPointer(index,
			attr.components, attr.type, GL_FALSE, 0, (void*) offset );

		if( glHasError("Error binding pointers to buffer") )
			return;

		offset += attr.data.size();
	}
}

//-----------------------------------//

void VertexBuffer::unbindGenericPointers()
{
	if( !built ) return;

	AttributeMap::const_iterator it;

	for( it = attributes.begin(); it != attributes.end(); it++ )
	{
		VertexAttribute::Enum index = it->first;
		glDisableVertexAttribArray(index);
	}
}

//-----------------------------------//

bool VertexBuffer::isValid() const
{
	// Check that we have a valid buffer.
	if( !glIsBuffer(id) )
	{
		LogWarn( "Vertex buffer is not valid" );
		return false;
	}

	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum index, const std::vector<Vector3>& data )
{
	built = false;

	Attribute attribute;
	attribute.components = 3;
	attribute.size = sizeof(float);
	attribute.type = GL_FLOAT;
	attribute.data.resize( data.size() * sizeof(Vector3) );
	
	if( data.size() != 0)
		memcpy( &attribute.data[0], &data[0], attribute.data.size() );
	
	attributes[index] = attribute;
	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum index, const std::vector<float>& data )
{
	built = false;

	Attribute attribute;
	attribute.components = 1;
	attribute.size = sizeof(float);
	attribute.type = GL_FLOAT;
	attribute.data.resize( data.size() * sizeof(float) );
	
	if( data.size() != 0)
		memcpy( &attribute.data[0], &data[0], attribute.data.size() );
	
	attributes[index] = attribute;
	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum index, const std::vector<int>& data )
{
	built = false;

	Attribute attribute;
	attribute.components = 1;
	attribute.size = sizeof(int);
	attribute.type = GL_INT;
	attribute.data.resize( data.size() * sizeof(int) );
	
	if( data.size() != 0)
		memcpy( &attribute.data[0], &data[0], attribute.data.size() );
	
	attributes[index] = attribute;
	return true;
}

//-----------------------------------//

bool VertexBuffer::set( VertexAttribute::Enum index, const std::vector<Color>& data )
{
	built = false;

	Attribute attribute;
	attribute.components = 4;
	attribute.size = sizeof(float);
	attribute.type = GL_FLOAT;
	attribute.data.resize( data.size() * sizeof(Color) );
	
	if( data.size() != 0)
		memcpy( &attribute.data[0], &data[0], attribute.data.size() );
	
	attributes[index] = attribute;
	return true;
}

//-----------------------------------//

bool VertexBuffer::build()
{
	bind();

	// Check that all vertex attributes elements are the same size.
	//if( !checkSize() )
	//{
	//	LogWarn( "Vertex buffers elements have different sizes" );
	//	return false;
	//}

	// Reserve space for all the vertex elements.
	glBufferData( GL_ARRAY_BUFFER, getSize(), nullptr, getGLBufferType() );

	if( glHasError("Could not allocate storage for buffer") )
		return false;

	int offset = 0;
	
	AttributeMap::const_iterator it;
	for( it = attributes.begin(); it != attributes.end(); it++ )
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
	for( it = attributes.begin(); it != attributes.end(); it++ )
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
	for( it = attributes.begin(); it != attributes.end(); it++ )
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
	{
		static std::vector<Vector3> nul;
		return nul;
	}
	
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

NAMESPACE_ENGINE_END

#endif