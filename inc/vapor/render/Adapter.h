/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace render {

/**
 * The adapter class just represents a graphics adapter (video card), and holds
 * data related to it that is interesting for the other classes, like driver
 * information and capabilities. Obviously this depends on the rendering API
 * the app is using at runtime, but I still have to check if  we need different
 * classes for different render devices. Hopefully not. :)
 */

class Adapter
{
public:

	Adapter() {};
	virtual ~Adapter() {};

	// Gets the version of the rendering API
	virtual const string getVersion() const = 0;

	// Gets driver information
	virtual const string getDriver() const = 0;
	
	// Gets the graphics adapter vendor
	virtual const string getVendor() const = 0;
	
	// Gets the graphics adapter name
	virtual const string getName() const = 0;

	// Gets the graphics adapter shading version
	virtual const string getShading() const = 0;

	// Logs adapter version information
	virtual void log() const = 0;
};

} } // end namespaces
