/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/RenderBuffer.h"

namespace vapor {

//-----------------------------------//

/**
 * FBOs are framebuffers object, an implementation of render buffers.
 */

class VAPOR_API FBO : public RenderBuffer
{
public:

	FBO(const Settings&);
	virtual ~FBO();

	// Binds/unbinds the FBO.
	void bind();
	void unbind();

	// Checks if the FBO is complete.
	bool check();

	// Updates the render target (usually swaps buffers).
	virtual void update();

	// Creates a new render buffer with the given components.
	void createRenderBuffer( int bufferComponents = RenderBufferType::Color | RenderBufferType::Depth );

	// Creates a render texture to this FBO.
	TexturePtr createRenderTexture( RenderBufferType::Enum = RenderBufferType::Color );

	// Attaches a render texture to this FBO.
	void attachRenderTexture(const TexturePtr& tex);

	// Gets the settings of this render target.
	GETTER(Settings, const Settings&, settings)

protected:

	void createRenderBufferStorage(int buffer, int type, int attachment);
	void setBufferState();
	bool checkSize();

	uint id;
	std::vector< uint > renderBuffers;
	std::vector< TexturePtr > textureBuffers;

	bool bound;
	bool valid;
	bool colorAttach;
};

TYPEDEF_PTR( FBO );

//-----------------------------------//

} // end namespace