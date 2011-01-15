/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Component.h"

namespace vapor {

//-----------------------------------//

class VAPOR_API Controller : public Component
{
	DECLARE_CLASS_()

public:

	virtual ~Controller();

	// Gets/sets if the controller is enabled.
	ACESSOR(Enabled, bool, enabled)

	// Updates the controller.
	virtual void update( double delta );
	
	// Implements the controller update logic.
	virtual void _update( double delta ) = 0;

protected:

	Controller();

	// If controller is disabled, it doesn't update.
	bool enabled;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Controller )
TYPEDEF_SHARED_WEAK_POINTER_FROM_TYPE( Controller )

//-----------------------------------//

} // end namespace