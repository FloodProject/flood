/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Component.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(Controller)

class API_ENGINE Controller : public Component
{
	REFLECT_DECLARE_OBJECT(Controller)

public:

	virtual ~Controller();

	// Gets/sets if the controller is enabled.
	GETTER(Enabled, bool, enabled)
	virtual void setEnabled(bool enabled);

	// Updates the controller.
	virtual void update( float delta );
	
	// Implements the controller update logic.
	virtual void _update( float delta ) = 0;

protected:

	Controller();

	// If controller is disabled, it doesn't update.
	bool enabled;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Controller )

//-----------------------------------//

NAMESPACE_ENGINE_END