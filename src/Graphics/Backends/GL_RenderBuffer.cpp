/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Graphics/API.h"
#include "Resources/Image.h"
#include "Resources/Buffer.h"
#include "GL_RenderBuffer.h"
#include "GL.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

GL_RenderBuffer::GL_RenderBuffer(const Settings& settings) 
	: RenderBuffer(settings)
	, bound(false)
	, valid(false)
	, colorAttach(false)
{
	glGenFramebuffersEXT(1, (GLuint*) &id);
	CheckLastErrorGL( "Could not create framebuffer object" );
}

//-----------------------------------//

GL_RenderBuffer::~GL_RenderBuffer()
{
	glDeleteFramebuffersEXT(1, (GLuint*) &id);

	for( size_t i = 0; i < renderBuffers.size(); i++ )
	{
		uint32 buffer = renderBuffers[i];

		glDeleteRenderbuffersEXT(1, (GLuint*) &buffer);
		CheckLastErrorGL( "Could not delete renderbuffer object" );
	}
}

//-----------------------------------//

void GL_RenderBuffer::bind()
{
	//if(bound) return;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	CheckLastErrorGL( "Could not bind framebuffer object" );

	setBufferState();
	bound = true;
}

//-----------------------------------//

void GL_RenderBuffer::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	CheckLastErrorGL( "Could not unbind framebuffer object" );
	
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);

	bound = false;
}

//-----------------------------------//

bool GL_RenderBuffer::check()
{
	bind();

	setBufferState();

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		LogWarn("GL_RenderBuffer error: %s", glErrorString(status) );
		return false;
	}

	unbind();
	
	valid = true;
	return true;
}

//-----------------------------------//

void GL_RenderBuffer::read(int8 attachment, std::vector<uint8>& data)
{
	const Vector2i& size = settings.getSize();

	data.resize( size.x*size.y*4 );

	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
}

//-----------------------------------//

void GL_RenderBuffer::setBufferState()
{
	if( colorAttach )
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_NONE);
	}
	else
	{
		// In case there is only a depth attachment in the GL_RenderBuffer,
		// then we need to setup the following OpenGL state.
	
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
}

//-----------------------------------//

TexturePtr GL_RenderBuffer::createRenderTexture( RenderBufferType::Enum type )
{
	PixelFormat::Enum format;

	if( type == RenderBufferType::Depth )
	{
		format = PixelFormat::Depth;
	}
	else
	{
		format = PixelFormat::R8G8B8A8;
		colorAttach = true;
	}

	Texture* tex = AllocateThis(Texture);
	tex->allocate(settings.getSize(), format);

	attachRenderTexture(tex);
	
	return tex;
}

//-----------------------------------//

void GL_RenderBuffer::attachRenderTexture(const TexturePtr& tex)
{
	bind();

	GLint attach = GL_COLOR_ATTACHMENT0_EXT;

	if( tex->getPixelFormat() == PixelFormat::Depth )
		attach = GL_DEPTH_ATTACHMENT_EXT;

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attach, GL_TEXTURE_2D, tex->getId(), 0);
	CheckLastErrorGL( "Could not attach texture into framebuffer object" );
	
	textureBuffers.push_back( tex );

	unbind();
}

//-----------------------------------//

void GL_RenderBuffer::createRenderBuffer( int bufferComponents )
{
	// Render buffers are just objects which are used to support
	// offscreen rendering, often for sections of the framebuffer which 
	// don’t have a texture format associated with them.

	if( !checkSize() ) return;

	GLuint renderBuffer;
	glGenRenderbuffersEXT(1, &renderBuffer);
	CheckLastErrorGL( "Could not generate renderbuffer object" );

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuffer);
	CheckLastErrorGL( "Could not bind renderbuffer object" );

	bind();

	if( bufferComponents & RenderBufferType::Depth )
	{
		createRenderBufferStorage(renderBuffer, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT_EXT);
	}
	if( bufferComponents & RenderBufferType::Color )
	{
		createRenderBufferStorage(renderBuffer, GL_RGBA, GL_COLOR_ATTACHMENT0_EXT);
		colorAttach = true;
	}
	if( bufferComponents & RenderBufferType::Stencil )
	{
		createRenderBufferStorage(renderBuffer, GL_STENCIL_INDEX, GL_STENCIL_ATTACHMENT_EXT);
		colorAttach = true;
	}

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	unbind();
}

//-----------------------------------//

void GL_RenderBuffer::createRenderBufferStorage(int buffer, int type, int attachment)
{
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, type, settings.width, settings.height);
	CheckLastErrorGL( "Could not create renderbuffer object storage" );

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, buffer);
	CheckLastErrorGL( "Could not attach renderbuffer into framebuffer object" );
}

//-----------------------------------//

bool GL_RenderBuffer::checkSize()
{
	// Check if the GL_RenderBuffer respect the maximum size defined by OpenGL.
	uint32 max = GL_MAX_RENDERBUFFER_SIZE_EXT;

	if(settings.width > max || settings.height > max)
	{
		LogWarn( "Invalid GL_RenderBuffer dimensions (OpenGL max: %d,%d)", max, max );
		return false;
	}

	return true;
}

//-----------------------------------//

void GL_RenderBuffer::update()
{

}

//-----------------------------------//

NAMESPACE_GRAPHICS_END