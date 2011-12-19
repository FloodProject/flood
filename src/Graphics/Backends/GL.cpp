/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/RenderBatch.h"
#include "Graphics/RenderCapabilities.h"

#ifdef ENABLE_RENDERER_OPENGL

#include "GL.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

#ifdef BUILD_DEBUG
bool CheckLastErrorGL( const char* msg )
{
	bool occured = false;
	uint numMaxErrors = 5;

	GLenum error = 0;
	while(((error = glGetError()) != GL_NO_ERROR) && (numMaxErrors-- > 0) )
	{
		LogAssert("OpenGL: %s", msg);
		occured = true;
	}

	return occured;
}
#else
#define CheckLastErrorGL(...) (false)
#endif

//-----------------------------------//

#ifdef BUILD_DEBUG

struct TokenString
{
	GLuint token;
	const char *string;
};

static const struct TokenString errors[] =
{
	{ GL_NO_ERROR, "No error" },
	{ GL_INVALID_ENUM, "Invalid enumerant" },
	{ GL_INVALID_VALUE, "Invalid value" },
	{ GL_INVALID_OPERATION, "Invalid operation" },
	{ GL_STACK_OVERFLOW, "Stack overflow" },
	{ GL_STACK_UNDERFLOW, "Stack underflow" },
	{ GL_OUT_OF_MEMORY, "Out of memory" },
	{ GL_TABLE_TOO_LARGE, "Table too large" },
	{ GL_INVALID_FRAMEBUFFER_OPERATION_EXT, "Invalid framebuffer operation" },
	{ GL_FRAMEBUFFER_COMPLETE_EXT, "Framebuffer complete" },
	{ GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT, "Framebuffer has incomplete attachment" },
	{ GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT, "Framebuffer is missing attachment" },
	{ GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT, "Framebuffer has incomplete dimensions" },
	{ GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT, "Framebuffer has incomplete formats" },
	{ GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT, "Framebuffer has incomplete draw buffer" },
	{ GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT, "Framebuffer has incomplete read buffer" },
	{ GL_FRAMEBUFFER_UNSUPPORTED_EXT, "Framebuffer" },
};

const char* glErrorString(GLenum errorCode)
{
	for (int i = 0; errors[i].string; i++) 
	{
		if(errors[i].token == errorCode)
			return errors[i].string;
	}

	return nullptr;
}

#endif

//-----------------------------------//

GLenum ConvertBufferTypeGL(VertexDataType::Enum type)
{
	switch(type)
	{
	case VertexDataType::Float:   return GL_FLOAT;
	case VertexDataType::Integer: return GL_INT;
	case VertexDataType::Byte:    return GL_UNSIGNED_BYTE;
	}

	LogAssert( "Unknown vertex buffer data type" );

	return GL_FLOAT;
}

//-----------------------------------//

int ConvertBufferGL(BufferUsage::Enum usage, BufferAccess::Enum access)
{
	if( usage == BufferUsage::Stream && access == BufferAccess::Read )
		return GL_STREAM_READ;
	
	else if( usage == BufferUsage::Stream && access == BufferAccess::Write )
		return GL_STREAM_DRAW;
	
	else if( usage == BufferUsage::Stream && access == BufferAccess::ReadWrite )
		return GL_STREAM_COPY;

	else if( usage == BufferUsage::Static && access == BufferAccess::Read )
		return GL_STATIC_READ;
	
	else if( usage == BufferUsage::Static && access == BufferAccess::Write )
		return GL_STATIC_DRAW;
	
	else if( usage == BufferUsage::Static && access == BufferAccess::ReadWrite )
		return GL_STATIC_COPY;

	else if( usage == BufferUsage::Dynamic && access == BufferAccess::Read )
		return GL_DYNAMIC_READ;
	
	else if( usage == BufferUsage::Dynamic && access == BufferAccess::Write )
		return GL_DYNAMIC_DRAW;
	
	else if( usage == BufferUsage::Dynamic && access == BufferAccess::ReadWrite )
		return GL_DYNAMIC_COPY;

	assert(0 && "Unknown GL buffer semantics");
	return GL_STREAM_READ;
}

//-----------------------------------//

void BindFixedVertexBufferDeclarations(VertexBuffer* vb)
{
	assert( vb->built );

	const GeometryBuffer* gb = vb->getGeometryBuffer();
	const std::vector<VertexElement>& vertexDecls = gb->declarations.decls;

	int offset = 0;

	for( size_t i = 0; i < vertexDecls.size(); ++i )
	{
		const VertexElement& decl = vertexDecls[i];
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

void UnbindFixedVertexBufferDeclarations(VertexBuffer* vb)
{
	const GeometryBuffer* gb = vb->getGeometryBuffer();
	const std::vector<VertexElement>& vertexDecls = gb->declarations.decls;

	for( size_t i = 0; i < vertexDecls.size(); i++ )
	{
		const VertexElement& decl = vertexDecls[i];
		
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

void BindGenericVertexBufferDeclarations(VertexBuffer* vb)
{
	const GeometryBuffer* gb = vb->getGeometryBuffer();
	const std::vector<VertexElement>& decls = gb->declarations.decls;

	for( size_t i = 0; i < decls.size(); i++ )
	{
		const VertexElement& decl = decls[i];
		int index = (int) decl.attribute;

		glEnableVertexAttribArray(index);

		CheckLastErrorGL("Error enabling vertex attribute array");

		GLenum type = ConvertBufferTypeGL(decl.type);
		
		glVertexAttribPointer(index, decl.components, type, GL_FALSE,
			decl.stride, (void*) decl.offset );

		CheckLastErrorGL("Error binding pointers to buffer");
	}
}

//-----------------------------------//

void UnbindGenericVertexBufferDeclarations(VertexBuffer* vb)
{
	const GeometryBuffer* gb = vb->getGeometryBuffer();
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

GLenum ConvertPrimitiveGL(PrimitiveType::Enum type)
{
	switch(type)
	{
	case PrimitiveType::LineLoop:      return GL_LINE_LOOP;
	case PrimitiveType::Lines:         return GL_LINES;
	case PrimitiveType::LineStrip:     return GL_LINE_STRIP;
	case PrimitiveType::Points:        return GL_POINTS;
	case PrimitiveType::Polygon:       return GL_POLYGON;
	case PrimitiveType::Quads:         return GL_QUADS;
	case PrimitiveType::QuadStrip:     return GL_QUAD_STRIP;
	case PrimitiveType::TriangleFan:   return GL_TRIANGLE_FAN;
	case PrimitiveType::Triangles:     return GL_TRIANGLES;
	case PrimitiveType::TriangleStrip: return GL_TRIANGLE_STRIP;
	}

	return GL_TRIANGLES;
}

//-----------------------------------//

GLenum ConvertPrimitiveRasterGL(PrimitiveRasterMode::Enum mode)
{
	switch(mode)
	{
	case PrimitiveRasterMode::Solid: return GL_FILL;
	case PrimitiveRasterMode::Wireframe: return GL_LINE;
	case PrimitiveRasterMode::Point: return GL_POINT;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

int ConvertTextureFilterFormatGL( TextureFilterMode::Enum format )
{
	switch( format )
	{
	case TextureFilterMode::Nearest: return GL_NEAREST;
	case TextureFilterMode::Linear: return GL_LINEAR;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

int ConvertTextureWrapFormatGL( TextureWrapMode::Enum format )
{
	switch( format )
	{
	case TextureWrapMode::Repeat: return GL_REPEAT;
	case TextureWrapMode::Clamp: return GL_CLAMP;
	case TextureWrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
	case TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLint ConvertTextureInternalFormatGL( PixelFormat::Enum format )
{
	switch( format )
	{
	case PixelFormat::R8G8B8A8: return GL_RGBA8;
	case PixelFormat::R8G8B8: return GL_RGB8;
	case PixelFormat::Depth: return GL_DEPTH_COMPONENT;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLint ConvertTextureSourceFormatGL( PixelFormat::Enum format )
{
	switch( format )
	{
	case PixelFormat::R8G8B8A8: return GL_RGBA;
	case PixelFormat::R8G8B8: return GL_RGB;
	case PixelFormat::Depth: return GL_DEPTH_COMPONENT;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLenum ConvertBlendSourceGL(BlendSource::Enum source)
{
	switch( source )
	{
	case BlendSource::Zero: return GL_ZERO;
	case BlendSource::One: return GL_ONE;
	case BlendSource::DestinationColor: return GL_DST_COLOR;
	case BlendSource::InverseDestinationColor: return GL_ONE_MINUS_DST_COLOR;
	case BlendSource::SourceAlpha: return GL_SRC_ALPHA;
	case BlendSource::InverseSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case BlendSource::DestinationAlpha: return GL_DST_ALPHA;
	case BlendSource::InverseDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case BlendSource::SourceAlphaSaturate: return GL_SRC_ALPHA_SATURATE;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLenum ConvertBlendDestinationGL(BlendDestination::Enum dest)
{
	switch( dest )
	{
	case BlendDestination::Zero: return GL_ZERO;
	case BlendDestination::One: return GL_ONE;
	case BlendDestination::SourceColor: return GL_SRC_COLOR;
	case BlendDestination::InverseSourceColor: return GL_ONE_MINUS_SRC_COLOR;
	case BlendDestination::SourceAlpha: return GL_SRC_ALPHA;
	case BlendDestination::InverseSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case BlendDestination::DestinationAlpha: return GL_DST_ALPHA;
	case BlendDestination::InverseDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLenum ConvertDepthModeGL(DepthCompare::Enum depth)
{
	switch( depth )
	{
	case DepthCompare::Never: return GL_NEVER;
	case DepthCompare::Less: return GL_LESS;
	case DepthCompare::Equal: return GL_EQUAL;
	case DepthCompare::LessOrEqual: return GL_LEQUAL;
	case DepthCompare::Greater: return GL_GREATER;
	case DepthCompare::NotEqual: return GL_NOTEQUAL;
	case DepthCompare::GreaterOrEqual: return GL_GEQUAL;
	case DepthCompare::Always: return GL_ALWAYS;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLenum ConvertShaderTypeGL( ShaderType::Enum type )
{
	switch( type )
	{
	case ShaderType::Vertex: return GL_VERTEX_SHADER;
	case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	case ShaderType::Geometry: return GL_GEOMETRY_SHADER_EXT;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

void ResetGL(RenderCapabilities* caps)
{
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );

	if(caps->supportsFixedPipeline)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}

	// Buffers
	if(caps->supportsVertexBuffers)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Textures
	for(int32 i = 0; i < caps->maxTextureUnits; ++i)
	{
		glActiveTexture( GL_TEXTURE0+i );
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Shaders
	if(caps->supportsShaders)
	{
		glUseProgram(0);
	}
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif