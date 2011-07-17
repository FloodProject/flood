/************************************************************************
*
*  vapor3D Engine © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/ReferenceCount.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class Adapter;
class RenderDevice;
class RenderTarget;
class BufferManager;
class ProgramManager;
class TextureManager;

struct Color;

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

	// Sets the current clear color.
	void setClearColor(const Color& color);

	// Gets the framebuffer color in the given pixel location.
	// Note that point (0,0) is located in the top left corner.
	Color getPixel(uint16 x, uint16 y) const;

	// Logs the GPU capabilities.
	void showCapabilities(Adapter*);

	// Gets and parses the OpenGL information.
	void checkCapabilities(Adapter*);

	// Checks that all needed OpenGL extensions are available.
	void checkExtensions();

	// Adapter information.
	Adapter* adapter;

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
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( RenderContext );

//-----------------------------------//

NAMESPACE_ENGINE_END