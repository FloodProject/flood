/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/RenderBackend.h"
#include "Graphics/RenderBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class VertexBuffer;
class IndexBuffer;
class RenderBuffer;

class Texture;
class Image;

class Shader;
class ShaderProgram;

class RenderBatch;
class RenderState;
class RenderView;
class RenderCapabilities;

class RenderBackend
{
public:

	virtual void init() = 0;
	virtual void cleanup() = 0;
	virtual void checkCapabilities(RenderCapabilities*) = 0;

	// Render commands.
	virtual void renderBatch(RenderBatch*) = 0;
	virtual void setupRenderState( const RenderState&, bool bindUniforms ) = 0;
	virtual void unsetupRenderState( const RenderState& ) = 0;

	// Framebuffers.
	virtual Color getPixel(uint16 x, uint16 y) = 0;
	virtual void setClearColor(Color color) = 0;
	virtual void setupRenderView(RenderView*) = 0;
	virtual void clearRenderView(RenderView*) = 0;

	// Vertex buffers.
	virtual VertexBuffer* createVertexBuffer() = 0;
	virtual void releaseVertexBuffer(VertexBuffer*) = 0;
	virtual void bindVertexBuffer(VertexBuffer*) = 0;
	virtual void unbindVertexBuffer(VertexBuffer*) = 0;
	virtual void buildVertexBuffer(VertexBuffer*) = 0;
	virtual void setupVertexBuffer(VertexBuffer*) = 0;

	// Index buffers.
	virtual IndexBuffer* createIndexBuffer() = 0;
	virtual void releaseIndexBuffer(IndexBuffer*) = 0;
	virtual void bindIndexBuffer(IndexBuffer*) = 0;
	virtual void unbindIndexBuffer(IndexBuffer*) = 0;
	virtual void buildIndexBuffer(IndexBuffer*) = 0;

	// Render buffers.
	virtual RenderBuffer* createRenderBuffer(const Settings&) = 0;

	// Textures.
	virtual Texture* createTexture() = 0;
	virtual void releaseTexture(Texture*) = 0;
	virtual void uploadTexture(Texture*) = 0;
	virtual void configureTexture(Texture*) = 0;
	virtual void bindTexture(Texture*) = 0;
	virtual void unbindTexture(Texture*) = 0;
	virtual Image* readTexture(Texture*) = 0;

	// Texture units.
	virtual void setupTextureUnit(Texture* texture, const TextureUnit& unit) = 0;
	virtual void undoTextureUnit(Texture* texture, const TextureUnit& unit) = 0;

	// Shaders.
	virtual ShaderProgram* createProgram() = 0;
	virtual ShaderProgram* createShader() = 0;
	virtual void releaseShader(ShaderProgram*) = 0;
	virtual void compileShader(ShaderProgram*) = 0;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END