/************************************************************************
*
*  vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class RenderCapabilities;
class RenderDevice;
class RenderTarget;
class RenderBackend;

class BufferManager;
class ProgramManager;
class TextureManager;

struct Color;
class RenderBuffer;
class Settings;

/**
 * Represents a rendering context.
 */

class API_RENDER RenderContext : public ReferenceCounted
{
public:

	RenderContext();
	~RenderContext();

	// Makes the context current.
	void makeCurrent();

	// Initializes the context.
	void init();

	// Resets the device to the initial state.
	void resetState();

	// Gets the framebuffer color in the given pixel location.
	// Note that point (0,0) is located in the top left corner.
	Color getPixel(uint16 x, uint16 y) const;

	// Logs the GPU capabilities.
	void showCapabilities(RenderCapabilities*);

	// Creates a new render buffer (offscreen render target).
	RenderBuffer* createRenderBuffer( const Settings& );

	// Render capabilities information.
	RenderCapabilities* caps;

	// Manages all the buffers.
	BufferManager* bufferManager;

	// Manages all the textures.
	TextureManager* textureManager;

	// Manages all the shaders.
	ProgramManager* programManager;

	// Keeps if the context initialization has already been done.
	bool initDone;

	// Context main target.
	RenderTarget* mainTarget;

	// Render backend.
	RenderBackend* backend;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( RenderContext );

//-----------------------------------//

NAMESPACE_GRAPHICS_END