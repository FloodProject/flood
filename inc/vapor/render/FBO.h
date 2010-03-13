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

namespace vapor { namespace render {

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
		Color = 1 << 0,
		Depth = 1 << 1,
		Stencil = 1 << 2
	};
}

//-----------------------------------//

/**
 * Framebuffer objects (FBOs), are render targets that you ask the engine
 * to render content into. There are 2 types of framebuffer render targets
 * you can attach, textures - also known as render-to-texture or RTT - and
 * render buffers, commonly called offscreen rendering. When you use RBs
 * you need to specify it's use type. You can render color, depth or even 
 * use it as a stencil buffer. There is also another technique called MRTs
 * (multiple render targets) which allows you to render into multiple FBOs
 * at the same time. FBOs have many uses: when you want to render from
 * another viewpoint and combine the contents in the main render target or
 * even render the scene and apply custom post-processing effects. 
 * They can also be used to implement regular shadow mapping.
 */

class VAPOR_API FBO : public RenderTarget, public ReferenceCounted
{
public:

	FBO(const Settings& settings);
	virtual ~FBO();

	void bind();

	void unbind();

	// Updates the render target (usually swaps buffers).
	virtual void update();

	// Sets this rendering target as the current.
	virtual void makeCurrent();

	void createRenderBuffer( int bufferComponents = 
		RenderBufferType::Color | RenderBufferType::Depth );

	void attachTexture(const TexturePtr& tex);

	// Gets the settings of this render target.
	virtual const Settings& getSettings();

protected:

	bool check();

	uint id;
	std::vector< uint > renderBuffers;
	std::vector< uint > textureBuffers;
	bool valid;

	// Holds the window settings
	Settings settings;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_CLASS( FBO );

//-----------------------------------//

} } // end namespaces