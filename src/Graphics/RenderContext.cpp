/************************************************************************
*
* vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderContext.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderBackend.h"
#include "Graphics/BufferManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/ProgramManager.h"
#include "Graphics/RenderCapabilities.h"
#include "Backends/GL.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderContext::RenderContext()
	: mainTarget(nullptr)
	, bufferManager(nullptr)
	, textureManager(nullptr)
	, programManager(nullptr)
	, caps(nullptr)
	, initDone(false)
	, backend(nullptr)
{
	backend = RenderCreateBackendGLES2();
}

//-----------------------------------//

RenderContext::~RenderContext()
{
	LogInfo("Destroying rendering context");

	Deallocate(backend);
	Deallocate(caps);
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

	caps = Allocate(GetRenderAllocator(), RenderCapabilities);
	backend->checkCapabilities(caps);

	bufferManager  = Allocate(GetRenderAllocator(), BufferManager);
	textureManager = Allocate(GetRenderAllocator(), TextureManager);
	programManager = Allocate(GetRenderAllocator(), ProgramManager);

	showCapabilities(caps);

	resetState();

	initDone = true;
}

//-----------------------------------//

void RenderContext::showCapabilities(RenderCapabilities* card) 
{
	if( card->name.empty() ) return;

	LogInfo( "Graphics RenderCapabilities: %s", card->name.c_str() );
	LogInfo( "OpenGL version: %s", card->driverVersion.c_str() );
	LogInfo( "OpenGL shading language: %s", card->shadingLanguageVersion.c_str() );
	
	LogInfo("Supports vertex buffers: ", card->supportsVertexBuffers ? "yes" : "no" );
	LogInfo( "Max texture size: %dx%d", card->maxTextureSize, card->maxTextureSize );
	LogInfo( "Max texture units: %d", card->maxTextureUnits );
	LogInfo( "Max vertex attributes: %d", card->maxAttribs );
}

//-----------------------------------//

void RenderContext::resetState()
{
	backend->setClearColor( Color::White );
	backend->init();
}

//-----------------------------------//

Color RenderContext::getPixel(uint16 x, uint16 y) const
{
	return backend->getPixel(x, y);
}

//-----------------------------------//

RenderBuffer* RenderContext::createRenderBuffer( const Settings& settings )
{
	return backend->createRenderBuffer(settings);
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END