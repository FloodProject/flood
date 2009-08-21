/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/InputManager.h"

namespace vapor {
	namespace input {

//-----------------------------------//

InputManager::InputManager()
{

}

//-----------------------------------//

InputManager::~InputManager()
{
	std::vector< input::Device* >::iterator it;
	
	for( it = devices.begin(); it != devices.end(); it++ )
	{
		delete (*it);
	}
}

//-----------------------------------//

void InputManager::addDevice( input::Device* device )
{
	if( !device )
	{
		warn( "input", "Tried to add an invalid input device" );
		return;
	}
	
	devices.push_back( device );

	info( "input", "Registered a new input device ('%s')", 
		DeviceType::getString( device->getType() ) );
}

//-----------------------------------//

Keyboard* InputManager::getKeyboard()
{
	std::vector< input::Device* >::iterator it;
	
	for( it = devices.begin(); it != devices.end(); it++ )
	{
		if( (*it)->getType() == DeviceType::Keyboard )
		{
			Keyboard* kbd = static_cast< Keyboard* > ( *it );
			return kbd;
		}
	}

	return nullptr;
}

//-----------------------------------//

void InputManager::processEvent( const input::Event& event )
{
	std::vector< input::Device* >::iterator it;
	
	for( it = devices.begin(); it != devices.end(); it++ )
	{
		(*it)->processEvent( event );
	}
}

//-----------------------------------//

} } // end namespaces