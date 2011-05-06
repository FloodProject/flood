/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Component.h"

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(Controller)

class VAPOR_API Controller : public Component
{
	REFLECT_DECLARE_OBJECT(Controller)

public:

	virtual ~Controller();

	// Gets/sets if the controller is enabled.
	ACESSOR(Enabled, bool, enabled)

	// Updates the controller.
	virtual void update( float delta );
	
	// Implements the controller update logic.
	virtual void _update( float delta ) = 0;

protected:

	Controller();

	// If controller is disabled, it doesn't update.
	bool enabled;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Controller )

//-----------------------------------//

} // end namespace