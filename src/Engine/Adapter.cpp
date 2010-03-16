/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Device.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

Adapter::Adapter()
{
	parseInfo();

	if(GLEW_ARB_vertex_buffer_object)
		supportsVBO = true;
	else
		supportsVBO = false;

	log();

	// TODO: add more Adapter caps	
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
		warn("gl::adapter", "Could not get GL renderer information");
	} else {
		name = tmp;
	}
	
	tmp = (const char*) glGetString(GL_VENDOR);
	if(tmp == nullptr) {
		warn("gl::adapter", "Could not get GL vendor information");
	} else {
		vendor = tmp;
	}

	tmp = (const char*) glGetString(GL_VERSION);
	if(tmp == nullptr) {
		warn("gl::adapter", "Could not get GL version information");
	} 
	else {
		gl = tmp;
		uint ch = gl.find_first_of("-");
		if(ch != std::string::npos) {
			driver = gl.substr(ch+1);
			gl = gl.substr(0, ch-1);
		} else {
			driver.clear();
		}
	}

	tmp = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
	if(tmp == nullptr) {
		warn("gl::adapter", "Could not get GLSL version information");
	} else {
		glsl = tmp;
		ch = glsl.find_first_of("-");
		glsl = glsl.substr(0, ch-1);
	}
}

//-----------------------------------//

void Adapter::log() const 
{
	if(name.empty()) return;

	const std::string& s = getShading();
	const std::string& d = getDriver();
	const std::string& g = getVersion();

	// log GL stuff
	info( "render::adapter", "Graphics adapter: %s", getName().c_str() );

	info( "render::adapter", "%s%s%s", 
		!g.empty() ? ("OpenGL " + g).c_str() : "",
		!s.empty() ? (" / GLSL " + s).c_str() : "",
		!d.empty() ? (" / driver: " + d).c_str() : "" );

	if( !supportsVBO )
	{
		error("gl::adapter", 
			"Your graphics device does not support VBOs (Vertex Buffer Objects).");
	}

	int maxAttribs;
	glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &maxAttribs );
	
	info( "render::adapter", "Max vertex attributes: %d", maxAttribs );
}

//-----------------------------------//

} } // end namespaces

#endif