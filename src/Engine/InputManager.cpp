/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/InputManager.h"

namespace vapor {

//-----------------------------------//

InputManager::~InputManager()
{
	foreach( Device* device, devices )
	{
		delete device;
	}
}

//-----------------------------------//

void InputManager::addDevice( Device* device )
{
	if( !device )
	{
		warn( "input", "Tried to add an invalid input device" );
		return;
	}
	
	devices.push_back( device );

	info( "input", "Registered a new input device: '%s'", 
		DeviceType::getString( device->getType() ).c_str() );
}

//-----------------------------------//

Keyboard* InputManager::getKeyboard() const
{
	foreach( Device* device, devices )
	{
		if( device->getType() == DeviceType::Keyboard )
		{
			return static_cast< Keyboard* > ( device );
		}
	}

	return nullptr;
}

//-----------------------------------//

Mouse* InputManager::getMouse() const
{
	foreach( Device* device, devices )
	{
		if( device->getType() == DeviceType::Mouse )
		{
			return static_cast< Mouse* > ( device );
		}
	}

	return nullptr;
}

//-----------------------------------//

const std::vector< Device* >& InputManager::getDevices() const
{
	return devices;
}

//-----------------------------------//

void InputManager::processEvent( const Event& event )
{
	foreach( Device* const device, devices )
	{
		device->processEvent( event );
	}
}

//-----------------------------------//

} // end namespace