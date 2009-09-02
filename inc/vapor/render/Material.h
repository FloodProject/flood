/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Rendering material.
 * TODO: Serialization?
 */

class Material
{
public:

	Material();
	~Material();

private:
};

//-----------------------------------//

typedef tr1::shared_ptr< Material > MaterialPtr;

//-----------------------------------//

} } // end namespaces