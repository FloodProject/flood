/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Target.h"
#include "vapor/render/Texture.h"

namespace vapor {

//-----------------------------------//

/**
 * Render buffers need to have renderable attachments. These attachments
 * can be of different types depending on your needs. This enum works
 * like a bitmask, so you can combine more than one type when attaching.
 */

namespace RenderBufferType
{
	enum Enum
	{
		Color	= 1 << 0,
		Depth	= 1 << 1,
		Stencil	= 1 << 2
	};
}

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

class VAPOR_API RenderBuffer : public RenderTarget
{
public:

	// Sets this rendering target as the current.
	void makeCurrent();

	// Binds/unbinds the render buffer.
	virtual void bind() = 0;
	virtual void unbind() = 0;

	// Checks if the render buffer is complete.
	virtual bool check() = 0;

	// Updates the render target.
	virtual void update() = 0;

	// Creates a new render buffer with the given components.
	virtual void createRenderBuffer( int bufferComponents = 
		RenderBufferType::Color | RenderBufferType::Depth ) = 0;

	// Creates a render texture.
	virtual TexturePtr createRenderTexture( 
		RenderBufferType::Enum = RenderBufferType::Color ) = 0;

	// Attaches a render texture.
	void attachRenderTexture(const TexturePtr& tex);

	// Gets the settings of this render target.
	IMPLEMENT_GETTER(Settings, const Settings&, settings)

protected:

	RenderBuffer(const Settings& settings);

	// Holds the window settings
	Settings settings;
};

TYPEDEF_PTR( RenderBuffer );

//-----------------------------------//

} // end namespace