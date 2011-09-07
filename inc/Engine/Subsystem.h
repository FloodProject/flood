/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Subsystems are managed by the engine. They are updated once in a while
 * (the subsystem can control the update rate) and are used mostly to 
 * implement the main services provided by the engine.
 */

class API_CORE NO_VTABLE Subsystem
{
	DECLARE_UNCOPYABLE(Subsystem)

public:

	virtual ~Subsystem() { }
	virtual void update() { }

protected:

	Subsystem() { }
};

//-----------------------------------//

NAMESPACE_ENGINE_END