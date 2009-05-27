/************************************************************************
*
* vaporEngine by triton © (2008-2009)
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
	
	// log GL capabilities
	info("render::opengl::adapter", "Graphics adapter: %s", getName().c_str());

	info("render::opengl::adapter", "Supports OpenGL %s / GLSL %s (driver: %s)",
		getVersion().c_str(), getShading().c_str(), getDriver().c_str());

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
 * so the also has to parse this to the right values.
 */

void GLAdapter::parseInfo()
{
	// get the name of the card
	name = (const char*) glGetString(GL_RENDERER);
	if(name.empty()) {
		warn("render::opengl::adapter", "Could not get GL renderer information");
		return;
	}
	
	vendor = (const char*) glGetString(GL_VENDOR);
	if(vendor.empty()) {
		warn("render::opengl::adapter", "Could not get GL vendor information");
		return;
	}

	driver = (const char*) glGetString(GL_VERSION);
	if(driver.empty()) {
		warn("render::opengl::adapter", "Could not get GL version information");
		return;
	}

	uint ch = driver.find_first_of("-");
	gl = driver.substr(0, ch-1);
	driver = driver.substr(ch+1);

	glsl = (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION);
	if(glsl.empty()) {
		warn("render::opengl::adapter", "Could not get GLSL version information");
		return;
	}

	ch = glsl.find_first_of("-");
	glsl = glsl.substr(0, ch-1);
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