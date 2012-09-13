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

int ConvertBufferGL(BufferUsage, BufferAccess);
GLenum ConvertPrimitiveGL(PrimitiveType);
GLenum ConvertPrimitiveRasterGL(PrimitiveRasterMode);

int ConvertTextureFilterFormatGL( TextureFilterMode );
int ConvertTextureWrapFormatGL( TextureWrapMode );
GLint ConvertTextureInternalFormatGL( PixelFormat );
GLint ConvertTextureSourceFormatGL( PixelFormat );
GLint ConvertTextureTargetGL( TextureTarget );

GLenum ConvertBlendSourceGL(BlendSource);
GLenum ConvertBlendDestinationGL(BlendDestination);
GLenum ConvertDepthModeGL(DepthCompare);

GLenum ConvertShaderTypeGL( ShaderType::Enum );

class RenderBackend;
RenderBackend* RenderCreateBackendGLES2();

//-----------------------------------//

NAMESPACE_GRAPHICS_END

#endif