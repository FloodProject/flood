/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/RenderTarget.h"
#include "Graphics/Texture.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Render buffers need to have renderable attachments. These attachments
 * can be of different types depending on your needs. This enum works
 * like a bitmask, so you can combine more than one type when attaching.
 */

enum struct RenderBufferType : uint8
{
	Color	= 1 << 0,
	Depth	= 1 << 1,
	Stencil	= 1 << 2
};

//-----------------------------------//

/**
 * Render buffers are offscreen render targets that you can render into.
 * There are 2 types of render targets attachments that you can use with
 * render buffers: textures (also known as render-to-textures or RTTs),
 * and render buffers. The attachments can have different types, so you
 * need to specify the type of attachments that you need. You can render
 * color, depth or even use them as stencil buffer. There is also another
 * technique called MRTs (multiple render targets) which allows you to 
 * render into multiple buffers at the same time.
 */

class API_GRAPHICS RenderBuffer : public RenderTarget
{
	DECLARE_UNCOPYABLE(RenderBuffer)

public:

	// Sets this rendering target as the current.
	void makeCurrent();

	// Binds the render buffer.
	virtual void bind() = 0;
	
	// Unbinds the render buffer.
	virtual void unbind() = 0;

	// Checks if the render buffer is complete.
	virtual bool check() = 0;

	// Updates the render buffer.
	virtual void update() = 0;

	// Reads the contents of the render buffer.
	virtual void read(int8 attachment, Vector<uint8>& data) = 0;

	// Reads the contents of the render buffer.
	virtual Image* readImage(int8 attachment) = 0;

	// Creates a new render buffer with the given components.
	virtual void createRenderBuffer( RenderBufferType ) = 0;

	// Creates a render texture.
	virtual TexturePtr createRenderTexture( RenderBufferType ) = 0;

	// Attaches a render texture.
	virtual void attachRenderTexture(const TexturePtr& tex) = 0;

	// Gets the settings of this render target.
	GETTER(Settings, const Settings&, settings)

protected:

	RenderBuffer(const Settings& settings);

	// Holds the target settings.
	Settings settings;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END