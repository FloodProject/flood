/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_RENDERER_OPENGL

#include "vapor/render/Adapter.h"

namespace vapor {
	namespace render {
		namespace opengl {

class GLAdapter : public render::Adapter
{
public:

	GLAdapter();
	virtual ~GLAdapter();

	// Gets the version of the rendering API
	virtual const string getVersion() const;

	// Gets driver information
	virtual const string getDriver() const;
	
	// Gets the graphics adapter vendor
	virtual const string getVendor() const;
	
	// Gets the graphics adapter name
	virtual const string getName() const;

	// Gets the GLSL version information
	virtual const string getShading() const;

	// Logs adapter version information
	virtual void log() const;

protected:

	// Gets and parses the OpenGL information
	void parseInfo();

	string name;
	string vendor;
	string driver;
	string glsl;
	string gl;
};

} } } // end namespaces

#endif