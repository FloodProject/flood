/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/Singleton.h"

namespace vapor { namespace render {

//-----------------------------------//	

/**
 * The adapter class just represents a graphics adapter (video card), 
 * and holds data related to it that is interesting for the other classes,
 * like driver information and capabilities. Obviously this depends on the
 * rendering API the app is using at runtime, but I still have to check if
 * we need different classes for different render devices. Hopefully not. :)
 */

class VAPOR_API Adapter : public Singleton<Adapter> 
{
public:

	Adapter();

	// Gets the version of the rendering API.
	IMPLEMENT_GETTER(Version, const std::string&, gl)

	// Gets driver information.
	IMPLEMENT_GETTER(Driver, const std::string&, driver)
	
	// Gets the graphics adapter vendor.
	IMPLEMENT_GETTER(Vendor, const std::string&, vendor)
	
	// Gets the graphics adapter name.
	IMPLEMENT_GETTER(Name, const std::string&, name)

	// Gets the GLSL version information.
	IMPLEMENT_GETTER(Shading, const std::string&, glsl)

	// Gets the maximum texture size.
	IMPLEMENT_GETTER(MaxTextureSize, int, maxTextureSize)

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

	int maxTextureSize;
	int maxAttribs;
	
	bool supportsVBO;
};

TYPEDEF_PTR(Adapter)

//-----------------------------------//

} } // end namespaces

#endif