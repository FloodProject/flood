/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org/
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"

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

	Adapter(void);
	virtual ~Adapter(void);

	//-----------------------------------//

	/**
	 * Gets driver information.
	 */
	virtual const char* getDriver();
	
	/**
	 * Gets the graphics adapter vendor.
	 */
	virtual const char* getVendor();
	
	/**
	 * Gets the graphics adapter name.
	 */
	virtual const char* getName();

protected:

	const char* _cardName;
	const char* _cardVendor;
	const char* _driverVersion;
};

} } // end namespaces
