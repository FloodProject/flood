/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//#ifdef ENABLE_RENDER

#include "Graphics/API.h"
#include "Graphics/RenderBackend.h"
#include "Graphics/RenderCapabilities.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/RenderBuffer.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderBatch.h"
#include "Graphics/RenderQueue.h"
#include "Graphics/RenderView.h"
#include "GL.h"
#include "GLSL_Program.h"
#include "GLSL_ShaderProgram.h"
#include "GL_RenderBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class RenderBackendGLES2 : public RenderBackend
{
public:

	void init() OVERRIDE;
	void cleanup() OVERRIDE;
	void checkCapabilities(RenderCapabilities*) OVERRIDE;
	
	void renderBatch(RenderBatch*) OVERRIDE;
	void setupRenderState( const RenderState&, bool bindUniforms ) OVERRIDE;
	void unsetupRenderState( const RenderState& ) OVERRIDE;
	void setupRenderView(RenderView*) OVERRIDE;
	void clearRenderView(RenderView*) OVERRIDE;

	// Framebuffers.
	Color getPixel(uint16 x, uint16 y) OVERRIDE;
	void setClearColor(Color color) OVERRIDE;

	// Vertex buffers.
	VertexBuffer* createVertexBuffer() OVERRIDE;
	void releaseVertexBuffer(VertexBuffer*) OVERRIDE;
	void bindVertexBuffer(VertexBuffer*) OVERRIDE;
	void unbindVertexBuffer(VertexBuffer*) OVERRIDE;
	void buildVertexBuffer(VertexBuffer*) OVERRIDE;
	void setupVertexBuffer(VertexBuffer*) OVERRIDE;

	// Index buffers.
	IndexBuffer* createIndexBuffer() OVERRIDE;
	void releaseIndexBuffer(IndexBuffer*) OVERRIDE;
	void bindIndexBuffer(IndexBuffer*) OVERRIDE;
	void unbindIndexBuffer(IndexBuffer*) OVERRIDE;
	void buildIndexBuffer(IndexBuffer*) OVERRIDE;

	// Render buffers.
	RenderBuffer* createRenderBuffer(const Settings&) OVERRIDE;

	// Textures.
	Texture* createTexture() OVERRIDE;
	void releaseTexture(Texture*) OVERRIDE;
	void uploadTexture(Texture*) OVERRIDE;
	void configureTexture(Texture*) OVERRIDE;
	void bindTexture(Texture*) OVERRIDE;
	void unbindTexture(Texture*) OVERRIDE;
	Image* readTexture(Texture*) OVERRIDE;

	// Shaders.
	Program* createProgram() OVERRIDE;
	ShaderProgram* createShader() OVERRIDE;
	void releaseShader(ShaderProgram*) OVERRIDE;
	void compileShader(ShaderProgram*) OVERRIDE;
};

//-----------------------------------//

RenderBackend* RenderCreateBackendGLES2()
{
	return AllocateRender(RenderBackendGLES2);
}

//-----------------------------------//

void RenderBackendGLES2::init()
{
	#pragma TODO("Remove GLEW and check needed extensions manually")

	// Initialize GLEW (OpenGL Extension Wrangler).
	GLenum status = glewInit();
	
	if( status != GLEW_OK )
	{
		const GLubyte* str = glewGetErrorString(status);
		LogError( "Failed to initialize GLEW: %s", str );
		return;
	}

	LogInfo( "Using GLEW version %s", glewGetString(GLEW_VERSION) );
}

//-----------------------------------//

void RenderBackendGLES2::cleanup()
{
}

//-----------------------------------//

void RenderBackendGLES2::setupRenderView(RenderView* view)
{
	setClearColor( view->getClearColor() );

	const Vector2i& origin = view->getOrigin();
	const Vector2i& size = view->getSize();

	glViewport( origin.x, origin.y, size.x, size.y );
}

//-----------------------------------//

void RenderBackendGLES2::clearRenderView(RenderView* view)
{
	const Vector2i& origin = view->getOrigin();
	const Vector2i& size = view->getSize();

	//glScissor(origin.x, origin.y, size.x, size.y);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	CheckLastErrorGL("Could not clear the render target");
}

//-----------------------------------//

void RenderBackendGLES2::renderBatch(RenderBatch* batch)
{
	PrimitiveRasterMode::Enum rasterMode = batch->getPrimitiveRasterMode();
	
	if( rasterMode != PrimitiveRasterMode::Solid )
	{
		GLenum mode = ConvertPrimitiveRasterGL(rasterMode);
		glPolygonMode( GL_FRONT_AND_BACK, mode );
	}

	GLenum primitiveType = ConvertPrimitiveGL( batch->getPrimitiveType() );
	const GeometryBuffer* gb = batch->getGeometryBuffer().get();

	if( !gb->isIndexed() )
	{
		uint32 numVertices = gb->getSizeVertices();
		
		if( numVertices )
		{
			glDrawArrays( primitiveType, 0, numVertices );
 			CheckLastErrorGL("Error drawing vertex buffer");
		}
	}
	else
	{
		GLenum indexType = (gb->indexSize == 16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
		int numIndices = gb->indexData.size() / (gb->indexSize / 8);

		glDrawElements( primitiveType, numIndices, indexType, 0 );
		CheckLastErrorGL("Error drawing index buffer");
	}

	if( rasterMode != PrimitiveRasterMode::Solid )
	{
		// Restore the polygon rendering mode.
		glPolygonMode( GL_FRONT_AND_BACK, PrimitiveRasterMode::Solid );
	}
}

//-----------------------------------//

void RenderBackendGLES2::setupRenderState( const RenderState& state, bool bindUniforms )
{
	//bindTextures(state, bindUniforms);
	
	Material* mat = state.material;

	if( mat->lineSmooth )
	{
		glEnable( GL_LINE_SMOOTH );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	}

	if( mat->lineWidth != Material::DefaultLineWidth )
		glLineWidth( mat->getLineWidth() );

	if( mat->depthCompare != DepthCompare::Less )
		glDepthFunc( mat->depthCompare );

	if( mat->depthRange != Vector2::UnitY )
		glDepthRange( mat->depthRange.x, mat->depthRange.y );

	if( mat->depthOffset != Vector2::Zero )
	{
		glPolygonOffset( mat->depthOffset.x, mat->depthOffset.y );
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	if( !mat->cullBackfaces )
		glDisable( GL_CULL_FACE );

	if( mat->alphaTest )
		glEnable( GL_ALPHA_TEST );

	if( !mat->depthTest )
		glDisable( GL_DEPTH_TEST );

	if( !mat->depthWrite )
		glDepthMask( GL_FALSE );

	if( mat->isBlendingEnabled() ) 
	{
		glEnable( GL_BLEND );
		glBlendFunc( mat->getBlendSource(), mat->getBlendDestination() );
	}
}

//-----------------------------------//

void RenderBackendGLES2::unsetupRenderState( const RenderState& state )
{
	Material* mat = state.material;

	if( mat->isBlendingEnabled() ) 
		glDisable( GL_BLEND );

	if( !mat->cullBackfaces )
		glEnable( GL_CULL_FACE );

	if( mat->alphaTest )
		glDisable( GL_ALPHA_TEST );

	if( mat->depthCompare != DepthCompare::Less )
		glDepthFunc( DepthCompare::Less );

	if( mat->depthRange != Vector2::UnitY )
		glDepthRange(0, 1);

	if( mat->depthOffset != Vector2::Zero )
	{
		glPolygonOffset( 0, 0 );
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	if( !mat->depthTest )
		glEnable( GL_DEPTH_TEST );

	if( !mat->depthWrite )
		glDepthMask( GL_TRUE );

	if( mat->lineSmooth )
		glDisable( GL_LINE_SMOOTH );

	if( mat->lineWidth != Material::DefaultLineWidth ) 
		glLineWidth( Material::DefaultLineWidth );

	//unbindTextures(mat);
}

//-----------------------------------//
#if 0
void RenderBackendGLES2::bindTextures(const RenderState& state, bool bindUniforms)
{
	TextureUnitMap& units = state.material->textureUnits;
	UniformBuffer* ub = state.renderable->getUniformBuffer().get();

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		const TextureUnit& unit = it->second;
		
		const ImageHandle& handle = unit.image;
		Image* image = handle.Resolve();

		Texture* texture = activeContext->textureManager->getTexture(image).get();
		if( !texture ) continue;

		uint8 index = unit.unit;
		renderBackend->bindTextureUnit(index);

		if(unit.overrideModes)
		{
			GLint filter = Texture::convertFilterFormat(unit.getFilterMode());
			glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, filter );
			glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, filter );

			GLint wrap = Texture::convertWrapFormat(unit.getWrapMode());
			glTexParameteri( texture->target, GL_TEXTURE_WRAP_S, wrap );
			glTexParameteri( texture->target, GL_TEXTURE_WRAP_T, wrap );
		}

		if( !bindUniforms ) continue;

		char s_TextureUniform[] = "vp_Texture0";
		size_t s_TextureUniformSize = ARRAY_SIZE(s_TextureUniform) - 1;

		// Build the uniform string without allocating memory.
		char indexChar = (index + '0');
		s_TextureUniform[s_TextureUniformSize] = indexChar;

		ub->setUniform( s_TextureUniform, (int32) index );
	}
}

//-----------------------------------//

void RenderBackendGLES2::unbindTextures(Material* material)
{
	TextureUnitMap& units = material->textureUnits;
	TextureManager* textureManager = activeContext->textureManager;

	TextureUnitMap::const_iterator it;
	for( it = units.begin(); it != units.end(); it++ )
	{
		const TextureUnit& unit = it->second;
		const ImageHandle& handle = unit.image;

		Texture* tex = textureManager->getTexture(handle.Resolve()).get();
		if( !tex ) continue;

		tex->unbind( it->first );
	}
}
#endif
//-----------------------------------//

void RenderBackendGLES2::checkCapabilities(RenderCapabilities* caps)
{
	caps->supportsShaders = !! GLEW_ARB_shading_language_100;
	caps->supportsVertexBuffers = !! GLEW_ARB_vertex_buffer_object;
	caps->supportsAnisotropic = !! GLEW_EXT_texture_filter_anisotropic;
	
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*) &caps->maxTextureSize );
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*) &caps->maxTextureUnits );
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, (GLint*) &caps->maxAttribs );

	// Gets the name of the card.
	caps->name = (const char*) glGetString(GL_RENDERER);
	caps->vendor = (const char*) glGetString(GL_VENDOR);
	caps->driverVersion = (const char*) glGetString(GL_VERSION);

	if(caps->supportsShaders)
		caps->shadingLanguageVersion = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);

	caps->supportsFixedPipeline = true;

#if 0
	if( caps->supportsShaders )
	{
		LogInfo( "Shaders support detected. Switching to forward shaders pipeline" );
		pipeline = RenderPipeline::ShaderForward;
	}
#endif
}

//-----------------------------------//

Color RenderBackendGLES2::getPixel(uint16 x, uint16 y)
{
	Color pick;
	
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &pick);
	CheckLastErrorGL("Error reading pixels from framebuffer");
	
	return pick;
}

//-----------------------------------//

void RenderBackendGLES2::setClearColor(Color color)
{
	glClearColor( color.r, color.g, color.b, color.a );
}

//-----------------------------------//

VertexBuffer* RenderBackendGLES2::createVertexBuffer()
{
	VertexBuffer* vb = AllocateRender(VertexBuffer);
	
	glGenBuffers( 1, (GLuint*) &vb->id );
	CheckLastErrorGL("Error generating a new buffer");

	return vb;
}

//-----------------------------------//

void RenderBackendGLES2::releaseVertexBuffer(VertexBuffer* vb)
{
	glDeleteBuffers( 1, (GLuint*) &vb->id );
	CheckLastErrorGL("Error deleting buffer");
}

//-----------------------------------//

void RenderBackendGLES2::bindVertexBuffer(VertexBuffer* vb)
{
	glBindBuffer( GL_ARRAY_BUFFER, vb->id );
	CheckLastErrorGL("Error binding vertex buffer");
}

//-----------------------------------//

void RenderBackendGLES2::unbindVertexBuffer(VertexBuffer* vb)
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	CheckLastErrorGL("Error unbinding vertex buffer");
}

//-----------------------------------//

void RenderBackendGLES2::buildVertexBuffer(VertexBuffer* vb)
{
	const GeometryBuffer* gb = vb->getGeometryBuffer();
	if( !gb ) return;

	const std::vector<uint8>& data = gb->data;
	if( data.empty() ) return;
	
	bindVertexBuffer(vb);

	GLenum usage = ConvertBufferGL(gb->usage, gb->access);

	// Upload all the vertex elements.
	glBufferData( GL_ARRAY_BUFFER, data.size(), &data.front(), usage );
	CheckLastErrorGL("Could not allocate storage for buffer");

	vb->built = true;

	unbindVertexBuffer(vb);
}

//-----------------------------------//

void RenderBackendGLES2::setupVertexBuffer(VertexBuffer* vb)
{
	#pragma TODO("Use VAOs to speed up this setup")
	BindGenericVertexBufferDeclarations(vb);
}

//-----------------------------------//

IndexBuffer* RenderBackendGLES2::createIndexBuffer()
{
	IndexBuffer* ib = AllocateRender(IndexBuffer);
	
	glGenBuffers( 1, (GLuint*) &ib->id );
	CheckLastErrorGL("Error generating a new index buffer");

	return ib;
}

//-----------------------------------//

void RenderBackendGLES2::releaseIndexBuffer(IndexBuffer* ib)
{
	glDeleteBuffers( 1, (GLuint*) &ib->id );
	CheckLastErrorGL("Error deleting index buffer");
}

//-----------------------------------//

void RenderBackendGLES2::bindIndexBuffer(IndexBuffer* ib)
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ib->id );
	CheckLastErrorGL( "Error binding index buffer" );
}

//-----------------------------------//

void RenderBackendGLES2::unbindIndexBuffer(IndexBuffer* ib)
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	CheckLastErrorGL( "Error unbinding index buffer" );
}

//-----------------------------------//

void RenderBackendGLES2::buildIndexBuffer(IndexBuffer* ib)
{
	//if( !gb ) return false;
	//assert( gb->isIndexed() );

	//GLsizeiptr indexSize = gb->indexData.size();
	//if( indexSize == 0 ) return false;

	//bind();

	//const GLvoid* data = &gb->indexData.front();

	// Reserve space for all the buffer elements.
	//GLenum usage = ConvertBufferGL(gb->getBufferUsage(), gb->getBufferAccess());
	//glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSize, data, usage );
	//CheckLastErrorGL("Could not buffer data in index buffer");

	ib->isBuilt = true;
}

//-----------------------------------//

RenderBuffer* RenderBackendGLES2::createRenderBuffer(const Settings& settings)
{
	RenderBuffer* renderBuffer = AllocateRender(GL_RenderBuffer, settings);
	return renderBuffer;
}

//-----------------------------------//

Texture* RenderBackendGLES2::createTexture()
{
	Texture* tex = AllocateRender(Texture);
	tex->target = GL_TEXTURE_2D;

	glGenTextures( 1, (GLuint*) &tex->id );
	CheckLastErrorGL("Could not generate a new texture object");

	return tex;
}

//-----------------------------------//

void RenderBackendGLES2::releaseTexture(Texture* tex)
{
	glDeleteTextures( 1, (GLuint*) &tex->id );
	CheckLastErrorGL("Could not delete texture object");

	LogDebug("Deleting texture from OpenGL");
}

//-----------------------------------//

void RenderBackendGLES2::bindTexture(Texture* tex)
{
}

//-----------------------------------//

void RenderBackendGLES2::unbindTexture(Texture* tex)
{
}

//-----------------------------------//

void RenderBackendGLES2::uploadTexture(Texture* tex)
{
	GLint internalFormat = ConvertTextureInternalFormatGL(tex->format);
	GLint sourceFormat = ConvertTextureSourceFormatGL(tex->format);
	
	bool hasData = tex->image && !tex->image->getBuffer().empty();
	uint8* data = hasData ? tex->image->getBuffer().data() : nullptr;

	bindTexture(tex);

	glTexImage2D( tex->target, 0, internalFormat, tex->width,
		tex->height, 0, sourceFormat, GL_UNSIGNED_BYTE, data );

	unbindTexture(tex);

	CheckLastErrorGL("Could not upload pixel data to texture object");

	tex->uploaded = true;
}

//-----------------------------------//

void RenderBackendGLES2::configureTexture(Texture* tex)
{
	bindTexture(tex);

#if 0
	if(GLEW_EXT_texture_filter_anisotropic)
		glTexParameterf( target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicFilter );
#endif

#if 0
	glTexParameteri( target, GL_TEXTURE_MIN_FILTER, ConvertTextureFilterFormatGL(minFilter) );
	glTexParameteri( target, GL_TEXTURE_MAG_FILTER, ConvertTextureFilterFormatGL(maxFilter) );

	glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
#endif

	#pragma TODO("Add support for mipmaps")

	if( glGenerateMipmap ) glGenerateMipmap(tex->target);

	glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 

	unbindTexture(tex);
}

//-----------------------------------//

Image* RenderBackendGLES2::readTexture(Texture* tex)
{
	std::vector<byte> data;
	data.resize( tex->getExpectedSize() );

	bindTexture(tex);
	
	glGetTexImage( tex->target, 0 /* base mipmap level */,
		ConvertTextureSourceFormatGL(tex->format), GL_UNSIGNED_BYTE, data.data() );
	
	if( CheckLastErrorGL("Could not read texture data") )
		return nullptr;

	unbindTexture(tex);

	Image* image = AllocateThis(Image);
	image->setWidth( tex->width );
	image->setHeight( tex->height );
	image->setPixelFormat( tex->format );
	image->setBuffer( data );

	return image;
}

//-----------------------------------//

Program* RenderBackendGLES2::createProgram()
{
	Program* program = AllocateRender(GLSL_Program);
	return program;
}

//-----------------------------------//

ShaderProgram* RenderBackendGLES2::createShader()
{
	ShaderProgram* shader = AllocateRender(GLSL_ShaderProgram);
	return shader;
}

//-----------------------------------//

void RenderBackendGLES2::releaseShader(ShaderProgram* shader)
{
	glDeleteShader(shader->id);
	CheckLastErrorGL("Could not delete shader object");
}

//-----------------------------------//

void RenderBackendGLES2::compileShader(ShaderProgram* shader)
{
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END

//#endif