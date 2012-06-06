/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_RENDERER_OPENGL

#define GLEW_STATIC
#include <GL/glew.h>

#include "Graphics/RenderBatch.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

// Handles OpenGL error states.
bool CheckLastErrorGL( const char* msg );

// Gets a string given an OpenGL error code.
const char* glErrorString(GLenum errorCode);

class VertexBuffer;
void BindFixedVertexBufferDeclarations(VertexBuffer*);
void UnbindFixedVertexBufferDeclarations(VertexBuffer*);
void BindGenericVertexBufferDeclarations(VertexBuffer*);
void UnbindGenericVertexBufferDeclarations(VertexBuffer*);

int ConvertBufferGL(BufferUsage::Enum, BufferAccess::Enum);
GLenum ConvertPrimitiveGL(PrimitiveType::Enum);
GLenum ConvertPrimitiveRasterGL(PrimitiveRasterMode::Enum);

int ConvertTextureFilterFormatGL( TextureFilterMode::Enum );
int ConvertTextureWrapFormatGL( TextureWrapMode::Enum );
GLint ConvertTextureInternalFormatGL( PixelFormat::Enum );
GLint ConvertTextureSourceFormatGL( PixelFormat::Enum );
GLint ConvertTextureTargetGL( Texture::TextureTarget );

GLenum ConvertBlendSourceGL(BlendSource::Enum);
GLenum ConvertBlendDestinationGL(BlendDestination::Enum);
GLenum ConvertDepthModeGL(DepthCompare::Enum);

GLenum ConvertShaderTypeGL( ShaderType::Enum );

class RenderBackend;
RenderBackend* RenderCreateBackendGLES2();

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif