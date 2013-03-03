/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/RenderContext.h"
#include "Graphics/RenderDevice.h"
#include "Graphics/RenderBackend.h"
#include "Graphics/BufferManager.h"
#include "Graphics/TextureManager.h"
#include "Graphics/ShaderProgramManager.h"
#include "Graphics/RenderCapabilities.h"
#include "Backends/GL.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

RenderContextSettings::RenderContextSettings()
	: bitsPerPixel(32)
	, stencilBits(8)
	, depthBits(24)
	, antialiasLevel(0)
{ }

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

	Deallocate(bufferManager);
	Deallocate(textureManager);
	Deallocate(programManager);
	Deallocate(caps);
	Deallocate(backend);
}

//-----------------------------------//

void RenderContext::makeCurrent()
{
	if( !mainTarget ) return;
	mainTarget->makeCurrent();

	if (!initDone)
		init();
}

//-----------------------------------//

void RenderContext::init()
{
	if( initDone ) return;

	LogInfo( "Initializing OpenGL rendering context" );

	caps = AllocateGraphics(RenderCapabilities);
	backend->checkCapabilities(caps);

	bufferManager = AllocateGraphics(BufferManager);
	bufferManager->setRenderBackend(backend);

	textureManager = AllocateGraphics(TextureManager);
	textureManager->setRenderBackend(backend);

	programManager = AllocateGraphics(ProgramManager, backend);

	showCapabilities(caps);

	resetState();

	initDone = true;
}

//-----------------------------------//

void RenderContext::showCapabilities(RenderCapabilities* card) 
{
	if( card->name.empty() ) return;

	LogInfo( "Graphics card: %s", card->name.c_str() );
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