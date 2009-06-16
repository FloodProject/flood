/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/render/gl/GL_Device.h"
#include "vapor/render/gl/GL_Adapter.h"

#ifdef VAPOR_RENDERER_OPENGL

namespace vapor {
	namespace render {
		namespace opengl {

//-----------------------------------//

GLAdapter::GLAdapter()
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

GLAdapter::~GLAdapter()
{

}

//-----------------------------------//

/**
 * Retrieves and parses the adapter information provided by OpenGL.
 * Some information is returned in the format: 
 *   <version number><space><vendor-specific information>		
 * so this function also has to parse the information.
 */

void GLAdapter::parseInfo()
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
		if(ch != string::npos) {
			driver = gl.substr(ch+1);
			gl = gl.substr(0, ch-1);
		} else {
			driver = "";
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

void GLAdapter::log() const 
{
	string s = getShading();
	string d = getDriver();
	string g = getVersion();

	// log GL stuff
	info("gl::adapter", "Graphics adapter: %s", 
		getName().c_str());

	info("render::opengl::adapter", "%s%s%s", 
		!g.empty() ? ("OpenGL " + g).c_str() : "",
		!s.empty() ? (" / GLSL " + s).c_str() : "",
		!d.empty() ? (" / driver: " + d).c_str() : "");

	if(!supportsVBO)
		error("gl::adapter", "Your graphics device does not support VBOs (Vertex Buffer Objects).");

}

//-----------------------------------//

const string GLAdapter::getVersion() const
{
	return gl;
}

//-----------------------------------//

const string GLAdapter::getDriver() const
{
	return driver;
}

//-----------------------------------//

const string GLAdapter::getVendor() const
{
	return vendor;
}

//-----------------------------------//

const string GLAdapter::getName() const
{
	return name;
}

//-----------------------------------//

const string GLAdapter::getShading() const
{
	return glsl;
}

//-----------------------------------//

} } } // end namespaces

#endif