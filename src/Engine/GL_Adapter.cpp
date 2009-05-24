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
	info("render::opengl::adapter", "Graphics adapter: %s", getName());
	info("render::opengl::adapter", "Supports OpenGL %s / GLSL %s", getDriver(), getShading());

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
	_name = glGetString(GL_RENDERER);
	if(!_name) warn("render::opengl::adapter", "Could not get GL renderer information");

	// TODO: parsing and error handling...
	
	_vendor = glGetString(GL_VENDOR);
	if(!_vendor) warn("render::opengl::adapter", "Could not get GL vendor information");

	_driver = glGetString(GL_VERSION);
	if(!_driver) warn("render::opengl::adapter", "Could not get GL version information");

	_glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if(!_glsl) warn("render::opengl::adapter", "Could not get GLSL version information");
}

//-----------------------------------//

const char* GLAdapter::getDriver() const
{
	return (const char*) _driver;
}

//-----------------------------------//

const char* GLAdapter::getVendor() const
{
	return (const char*) _vendor;
}

//-----------------------------------//

const char* GLAdapter::getName() const
{
	return (const char*) _name;
}

//-----------------------------------//

const char* GLAdapter::getShading() const
{
	return (const char*) _glsl;
}

//-----------------------------------//

} } } // end namespaces

#endif