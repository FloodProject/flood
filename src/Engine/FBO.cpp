/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/FBO.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

FBO::FBO(const Settings& settings) 
	: settings( settings ), valid( false )
{
	glGenFramebuffersEXT(1, &id);
	glHasError( "Could not create framebuffer object" );
}

//-----------------------------------//

FBO::~FBO()
{
	glDeleteFramebuffersEXT(1, &id);

	foreach( uint rb, renderBuffers )
	{
		glDeleteRenderbuffersEXT(1, &rb);
		glHasError( "Could not delete renderbuffer object" );
	}
}

//-----------------------------------//

void FBO::makeCurrent()
{
	bind();
}

//-----------------------------------//

void FBO::bind()
{
	if( !valid ) 
	{
		check();
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	glHasError( "Could not bind framebuffer object" );
}

//-----------------------------------//

void FBO::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glHasError( "Could not unbind framebuffer object" );	
}

//-----------------------------------//

bool FBO::check()
{
	GLenum status;
	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

	if( status == GL_FRAMEBUFFER_UNSUPPORTED_EXT )
	{
		return false;
	}
	
	valid = true;
	return true;
}

//-----------------------------------//

void FBO::createRenderBuffer( int bufferComponents )
{
	// Render buffers are used are just objects which are used to support
	// offscreen rendering, often for sections of the framebuffer which 
	// don’t have a texture format associated with them.

	uint w = settings.getWidth();
	uint h = settings.getHeight();

	uint renderBuffer;
	glGenRenderbuffersEXT(1, &renderBuffer);
	glHasError( "Could not generate renderbuffer object" );

	renderBuffers.push_back( renderBuffer );

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderBuffer);
	glHasError( "Could not bind renderbuffer object" );

	// Check if the FBO sizes respect the maximum defined by OpenGL.

	uint maxSize = GL_MAX_RENDERBUFFER_SIZE_EXT;
	if( (w > maxSize) || (h > maxSize) )
	{
		warn( "gl", "Invalid FBO dimensions (OpenGL max: %d,%d)",
			GL_MAX_RENDERBUFFER_SIZE_EXT );
	}

	bind();

	if( bufferComponents & RenderBufferType::Color )
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, w, h);
		glHasError( "Could not create renderbuffer object storage" );

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, 
			GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, renderBuffer);
		glHasError( "Could not attach renderbuffer into framebuffer object" );
	}

	if( bufferComponents & RenderBufferType::Depth )
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
		glHasError( "Could not create renderbuffer object storage" );

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
			GL_RENDERBUFFER_EXT, renderBuffer);
		glHasError( "Could not attach renderbuffer into framebuffer object" );
	}

	if( bufferComponents & RenderBufferType::Stencil )
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX, w, h);
		glHasError( "Could not create renderbuffer object storage" );

		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, 
			GL_RENDERBUFFER_EXT, renderBuffer);
		glHasError( "Could not attach renderbuffer into framebuffer object" );
	}
}

//-----------------------------------//

TexturePtr FBO::createRenderTexture()
{
	TexturePtr tex( new Texture(settings) );
	attachRenderTexture(tex);
	return tex;
}

//-----------------------------------//

void FBO::attachRenderTexture(const TexturePtr& tex)
{
	bind();

	// Ensure texture has been allocated.
	tex->upload();

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex->id(), 0);
	glHasError( "Could not attach texture into framebuffer object" );

	textureBuffers.push_back( tex->id() );
}

//-----------------------------------//

void FBO::update()
{
}

//-----------------------------------//

} } // end namespaces