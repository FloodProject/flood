/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/FBO.h"
#include "Render/GL.h"
#include "Resources/Image.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

FBO::FBO(const Settings& settings) 
	: RenderBuffer(settings)
	, bound(false)
	, valid(false)
	, colorAttach(false)
{
	glGenFramebuffersEXT(1, (GLuint*) &id);
	CheckLastErrorGL( "Could not create framebuffer object" );
}

//-----------------------------------//

FBO::~FBO()
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

void FBO::bind()
{
	//if(bound) return;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	CheckLastErrorGL( "Could not bind framebuffer object" );

	setBufferState();
	bound = true;
}

//-----------------------------------//

void FBO::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	CheckLastErrorGL( "Could not unbind framebuffer object" );
	
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);

	bound = false;
}

//-----------------------------------//

bool FBO::check()
{
	bind();

	setBufferState();

	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		LogWarn("FBO error: %s", glErrorString(status) );
		return false;
	}

	unbind();
	
	valid = true;
	return true;
}

//-----------------------------------//

void FBO::read(int8 attachment, std::vector<uint8>& data)
{
	const Vector2i& size = settings.getSize();

	data.resize( size.x*size.y*4 );

	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
}

//-----------------------------------//

void FBO::setBufferState()
{
	if( colorAttach )
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_NONE);
	}
	else
	{
		// In case there is only a depth attachment in the FBO,
		// then we need to setup the following OpenGL state.
	
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
}

//-----------------------------------//

TexturePtr FBO::createRenderTexture( RenderBufferType::Enum type )
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

	TexturePtr tex = AllocateThis(Texture, settings, format);
	attachRenderTexture(tex);
	
	return tex;
}

//-----------------------------------//

void FBO::attachRenderTexture(const TexturePtr& tex)
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

void FBO::createRenderBuffer( int bufferComponents )
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

void FBO::createRenderBufferStorage(int buffer, int type, int attachment)
{
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, type, settings.width, settings.height);
	CheckLastErrorGL( "Could not create renderbuffer object storage" );

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment, GL_RENDERBUFFER_EXT, buffer);
	CheckLastErrorGL( "Could not attach renderbuffer into framebuffer object" );
}

//-----------------------------------//

bool FBO::checkSize()
{
	// Check if the FBO respect the maximum size defined by OpenGL.
	uint32 max = GL_MAX_RENDERBUFFER_SIZE_EXT;

	if(settings.width > max || settings.height > max)
	{
		LogWarn( "Invalid FBO dimensions (OpenGL max: %d,%d)", max, max );
		return false;
	}

	return true;
}

//-----------------------------------//

void FBO::update()
{

}

//-----------------------------------//

NAMESPACE_ENGINE_END