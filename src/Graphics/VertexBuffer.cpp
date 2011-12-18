/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "Graphics/VertexBuffer.h"
#include "Graphics/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

VertexBuffer::VertexBuffer() : built(false), gb(nullptr)
{
}

//-----------------------------------//

VertexBuffer::~VertexBuffer()
{
}

//-----------------------------------//

bool VertexBuffer::bind()
{
	glBindBuffer( GL_ARRAY_BUFFER, id );
	return !CheckLastErrorGL("Error binding vertex buffer");
}

//-----------------------------------//

bool VertexBuffer::unbind()
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	return !CheckLastErrorGL("Error unbinding vertex buffer");
}

//-----------------------------------//

static GLenum ConvertBufferTypeGL(VertexType::Enum type)
{
	switch(type)
	{
	case VertexType::Float: return GL_FLOAT;
	case VertexType::Integer: return GL_INT;
	case VertexType::Byte: return GL_UNSIGNED_BYTE;
	}

	LogAssert( "Unknown vertex buffer data type" );

	return GL_FLOAT;
}

//-----------------------------------//

void VertexBuffer::bindPointers()
{
	if( !built || !gb ) return;

	const std::vector<VertexElement>& decls = gb->declarations.decls;
	int offset = 0;

	for( size_t i = 0; i < decls.size(); i++ )
	{
		const VertexElement& decl = decls[i];
		GLenum type = ConvertBufferTypeGL(decl.type);

		switch(decl.attribute)
		{
		case VertexAttribute::Position:
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(decl.components, type, decl.stride, (void*) offset);
			break;
		case VertexAttribute::Normal:
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(type, decl.stride, (void*) offset);
			break;
		case VertexAttribute::Color:
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(decl.components, type, decl.stride, (void*) offset);
			break;
		case VertexAttribute::TexCoord0:
		case VertexAttribute::TexCoord1:
		case VertexAttribute::TexCoord2:
		case VertexAttribute::TexCoord3:
			int texIndex = (int) decl.attribute - (int) VertexAttribute::TexCoord0;
			glClientActiveTexture(GL_TEXTURE0 + texIndex);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(decl.components, type, 0, (void*) offset);
			break;
		}
	}
}

//-----------------------------------//

void VertexBuffer::unbindPointers()	
{
	if( !built ) return;

	const std::vector<VertexElement>& decls = gb->declarations.decls;

	for( size_t i = 0; i < decls.size(); i++ )
	{
		const VertexElement& decl = decls[i];
		
		switch(decl.attribute)
		{
		case VertexAttribute::Position:
			glDisableClientState(GL_VERTEX_ARRAY);
			break;
		case VertexAttribute::Normal:
			glDisableClientState(GL_NORMAL_ARRAY);
			break;
		case VertexAttribute::Color:
			glDisableClientState(GL_COLOR_ARRAY);
			break;
		case VertexAttribute::TexCoord0:
		case VertexAttribute::TexCoord1:
		case VertexAttribute::TexCoord2:
		case VertexAttribute::TexCoord3:
			int texIndex = (int) decl.attribute - (int) VertexAttribute::TexCoord0;
			glClientActiveTexture(GL_TEXTURE0 + texIndex);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
}

//-----------------------------------//

void VertexBuffer::bindGenericPointers()
{
	if( !built || !gb ) return;

	const std::vector<VertexElement>& decls = gb->declarations.decls;

	for( size_t i = 0; i < decls.size(); i++ )
	{
		const VertexElement& decl = decls[i];
		int index = (int) decl.attribute;

		glEnableVertexAttribArray(index);

		if( CheckLastErrorGL("Error enabling vertex attribute array") )
			return;

		GLenum type = ConvertBufferTypeGL(decl.type);
		
		glVertexAttribPointer(index, decl.components, type, GL_FALSE,
			decl.stride, (void*) decl.offset );

		CheckLastErrorGL("Error binding pointers to buffer");
	}
}

//-----------------------------------//

void VertexBuffer::unbindGenericPointers()
{
	if( !built || !gb ) return;

	const std::vector<VertexElement>& decls = gb->declarations.decls;

	for( size_t i = 0; i < decls.size(); i++ )
	{
		const VertexElement& decl = decls[i];

		int index = (int) decl.attribute;
		glDisableVertexAttribArray(index);

		CheckLastErrorGL("Error disabling vertex attribute array");
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

bool VertexBuffer::build(const GeometryBuffer* gb)
{
	if( !gb ) return false;

	const std::vector<uint8>& data = gb->data;
	if( data.empty() ) return false;
	
	bind();

	GLenum usage = ConvertBufferGL(gb->getBufferUsage(), gb->getBufferAccess());

	// Upload all the vertex elements.
	glBufferData( GL_ARRAY_BUFFER, data.size(), &data.front(), usage );

	if( CheckLastErrorGL("Could not allocate storage for buffer") )
		return false;

	built = true;
	return true;
}

//-----------------------------------//

void VertexBuffer::forceRebuild()
{
	built = false;
}

//-----------------------------------//

bool VertexBuffer::isBuilt() const
{
	return built;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif