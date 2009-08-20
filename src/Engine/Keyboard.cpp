/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/Keyboard.h"
#include "vapor/input/Type.h"

namespace vapor {
	namespace input {

//-----------------------------------//

Keyboard::Keyboard()
: keyState(Keys::Pause, false), lastKey(Keys::Space)
{

}

//-----------------------------------//

Keyboard::~Keyboard()
{

}

//-----------------------------------//

const input::DeviceType::Enum Keyboard::getType()
{
	return input::DeviceType::Keyboard; 
}

//-----------------------------------//

void Keyboard::processEvent( const input::Event& event )
{
	if( event.deviceType != DeviceType::Keyboard )
	{
		return;
	}

	const KeyEvent& kevt = static_cast< const KeyEvent& > ( event );
	switch(kevt.eventType)
		{
		case KeyboardEvent::KeyPressed:
			keyPressed(kevt.keyCode);
			//TODO: Maybe add the rest of the struct's info, although it doesn't seem necessary
			break;
		case KeyboardEvent::KeyReleased:
			keyReleased(kevt.keyCode);
			break;

		}
}

//-----------------------------------//

KeyEvent::KeyEvent(Keys::Enum keyCode, bool alt, bool shift, bool ctrl, KeyboardEvent::Enum eventType):
	Event( DeviceType::Keyboard ), keyCode( keyCode ), altPressed( alt ),
		shiftPressed( shift ), ctrlPressed( ctrl ), eventType( eventType )
{

}

//-----------------------------------//

Keys::Enum Keyboard::lastKeyPressed()
{
	return lastKey;
}

//-----------------------------------//

KeyEvent Keyboard::infoLastKeyPressed()
{
	bool alt, shift, ctrl;
	alt = keyState[Keys::LAlt] || keyState[Keys::RAlt];
	shift = keyState[Keys::LShift] || keyState[Keys::RShift];
	ctrl = keyState[Keys::LControl] || keyState[Keys::RControl];
	KeyEvent keyinfo(lastKey, alt, shift, ctrl);
	return keyinfo;
}

//-----------------------------------//

bool Keyboard::isKeyPressed(Keys::Enum keycode)
{
	return keyState[keycode];
}

//-----------------------------------//

void Keyboard::keyPressed(Keys::Enum keycode)
{
	keyState[keycode] = true;
	lastKey = keycode;
}

//-----------------------------------//

void Keyboard::keyReleased(Keys::Enum keycode)
{
	keyState[keycode] = false;

}	

//-----------------------------------//
	}
} // end namespaces