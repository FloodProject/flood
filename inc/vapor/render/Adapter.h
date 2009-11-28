/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/Platform.h"

namespace vapor { namespace render {

//-----------------------------------//	

/**
 * The adapter class just represents a graphics adapter (video card), and holds
 * data related to it that is interesting for the other classes, like driver
 * information and capabilities. Obviously this depends on the rendering API
 * the app is using at runtime, but I still have to check if  we need different
 * classes for different render devices. Hopefully not. :)
 */

class VAPOR_API Adapter
{
public:

	Adapter();
	~Adapter();

	// Gets the version of the rendering API
	const std::string getVersion() const;

	// Gets driver information
	const std::string getDriver() const;
	
	// Gets the graphics adapter vendor
	const std::string getVendor() const;
	
	// Gets the graphics adapter name
	const std::string getName() const;

	// Gets the GLSL version information
	const std::string getShading() const;

	// Logs adapter version information
	void log() const;

protected:

	// Gets and parses the OpenGL information
	void parseInfo();

	std::string name;
	std::string vendor;
	std::string driver;
	std::string glsl;
	std::string gl;

	bool supportsVBO;
};

//-----------------------------------//

} } // end namespaces

#endif