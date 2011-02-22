/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "Render/Adapter.h"
#include "Render/GL.h"

namespace vapor {

//-----------------------------------//

Adapter::Adapter()
{
	parseInfo();

	supportsShaders = !! GLEW_ARB_shading_language_100;
	supportsVertexBuffers = !! GLEW_ARB_vertex_buffer_object;
	supportsAnisotropic = !! GLEW_EXT_texture_filter_anisotropic;

	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxAttribs );

	log();
}

//-----------------------------------//

/**
 * Retrieves and parses the adapter information provided by OpenGL.
 * Some information is returned in the format: 
 *   <version number><space><vendor-specific information>		
 * so this function also has to parse the information.
 */

void Adapter::parseInfo()
{
	const char* tmp = nullptr;
	uint ch;

	// get the name of the card
	tmp = (const char*) glGetString(GL_RENDERER);
	if(tmp == nullptr) {
		Log::warn("Could not get GL renderer information");
	} else {
		name = tmp;
	}
	
	tmp = (const char*) glGetString(GL_VENDOR);
	if(tmp == nullptr) {
		Log::warn("Could not get GL vendor information");
	} else {
		vendor = tmp;
	}

	tmp = (const char*) glGetString(GL_VERSION);
	if(tmp == nullptr) {
		Log::warn("Could not get GL version information");
	} 
	else {
		gl = tmp;
		size_t ch = gl.find_first_of("-");
		if(ch != std::string::npos) {
			driver = gl.substr(ch+1);
			gl = gl.substr(0, ch-1);
		} else {
			driver.clear();
		}
	}

	if(supportsShaders)
	{
		tmp = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
		if(tmp == nullptr) {
			Log::warn("Could not get GLSL version information");
		} else {
			glsl = tmp;
			ch = glsl.find_first_of("-");
			glsl = glsl.substr(0, ch-1);
		}
	}
}

//-----------------------------------//

void Adapter::log() const 
{
	if( name.empty() )
		return;

	Log::info( "Graphics adapter: %s", name.c_str() );

	Log::info( "%s%s%s", 
		!gl.empty() ? ("OpenGL " + gl).c_str() : "",
		!glsl.empty() ? (" / GLSL " + glsl).c_str() : "",
		!driver.empty() ? (" / driver: " + driver).c_str() : "" );

	if( !supportsVertexBuffers )
		Log::error("Your graphics adapter does not support Vertex Buffer Objects");

	Log::info( "Max texture size: %dx%d", maxTextureSize, maxTextureSize );
	Log::info( "Max vertex attributes: %d", maxAttribs );
}

//-----------------------------------//

} // end namespace

#endif