/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/RenderContext.h"
#include "Render/Device.h"
#include "Render/BufferManager.h"
#include "Render/TextureManager.h"
#include "Render/ProgramManager.h"
#include "Render/Adapter.h"
#include "Render/GL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

RenderContext::RenderContext()
	: mainTarget(nullptr)
	, bufferManager(nullptr)
	, textureManager(nullptr)
	, programManager(nullptr)
	, adapter(nullptr)
	, initDone(false)
{
}

//-----------------------------------//

RenderContext::~RenderContext()
{
	LogInfo("Destroying OpenGL rendering context");

	Deallocate(adapter);
	Deallocate(bufferManager);
	Deallocate(textureManager);
	Deallocate(programManager);
}

//-----------------------------------//

void RenderContext::makeCurrent()
{
	if( !mainTarget ) return;
	mainTarget->makeCurrent();
}

//-----------------------------------//

void RenderContext::init()
{
	if( initDone ) return;

	LogInfo( "Initializing OpenGL rendering context" );

	checkExtensions();

	adapter = Allocate(Adapter, GetRenderAllocator());
	bufferManager  = Allocate(BufferManager, GetRenderAllocator());
	textureManager = Allocate(TextureManager, GetRenderAllocator());
	programManager = Allocate(ProgramManager, GetRenderAllocator());

	checkCapabilities(adapter);
	showCapabilities(adapter);

	resetState();

	initDone = true;
}

//-----------------------------------//

void RenderContext::checkCapabilities(Adapter* adapter)
{
	adapter->supportsShaders = !! GLEW_ARB_shading_language_100;
	adapter->supportsVertexBuffers = !! GLEW_ARB_vertex_buffer_object;
	adapter->supportsAnisotropic = !! GLEW_EXT_texture_filter_anisotropic;
	
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*) &adapter->maxTextureSize );
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*) &adapter->maxTextureUnits );
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, (GLint*) &adapter->maxAttribs );

	// Gets the name of the card.
	adapter->name = (const char*) glGetString(GL_RENDERER);
	adapter->vendor = (const char*) glGetString(GL_VENDOR);
	adapter->driverVersion = (const char*) glGetString(GL_VERSION);

	if(adapter->supportsShaders)
		adapter->shadingLanguageVersion = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);

	adapter->supportsFixedPipeline = true;

#if 0
	if( adapter->supportsShaders )
	{
		LogInfo( "Shaders support detected. Switching to forward shaders pipeline" );
		pipeline = RenderPipeline::ShaderForward;
	}
#endif
}

//-----------------------------------//

void RenderContext::showCapabilities(Adapter* card) 
{
	if( card->name.empty() ) return;

	LogInfo( "Graphics adapter: %s", card->name.c_str() );
	LogInfo( "OpenGL version: %s", card->driverVersion.c_str() );
	LogInfo( "OpenGL shading language: %s", card->shadingLanguageVersion.c_str() );
	
	LogInfo("Supports vertex buffers: ", card->supportsVertexBuffers ? "yes" : "no" );
	LogInfo( "Max texture size: %dx%d", card->maxTextureSize, card->maxTextureSize );
	LogInfo( "Max texture units: %d", card->maxTextureUnits );
	LogInfo( "Max vertex attributes: %d", card->maxAttribs );
}

//-----------------------------------//

void RenderContext::checkExtensions()
{
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

void RenderContext::resetState()
{
	setClearColor( Color::White );

	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );

	if(adapter->supportsFixedPipeline)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
	}

	// Buffers
	if(adapter->supportsVertexBuffers)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Textures
	for(int32 i = 0; i < adapter->maxTextureUnits; i++)
	{
		glActiveTexture( GL_TEXTURE0+i );
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Shaders
	if(adapter->supportsShaders)
	{
		glUseProgram(0);
	}
}

//-----------------------------------//

Color RenderContext::getPixel(uint16 x, uint16 y) const
{
	Color pick;
	
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &pick);

	if( glHasError("Error reading pixels from framebuffer") )
		return Color::White;
	
	return pick;
}

//-----------------------------------//

void RenderContext::setClearColor(const Color& color)
{
	glClearColor( color.r, color.g, color.b, color.a );
}

//-----------------------------------//

NAMESPACE_ENGINE_END