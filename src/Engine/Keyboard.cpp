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
	: keyState(Keys::Pause, false)
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
}

//-----------------------------------//

KeyEvent::KeyEvent(Keys::Enum keyCode, bool alt, bool shift, bool ctrl)
	: Event( DeviceType::Keyboard ), keyCode( keyCode ), 
		altPressed( alt ), shiftPressed( shift ), ctrlPressed( ctrl )
{

}

//-----------------------------------//

//Keys::Enum Keyboard::lastKeyPressed()
//{
//	return pressedKeys.back();
//}
//
////-----------------------------------//
//
//KeyInfo Keyboard::infoLastKeyPressed()
//{
//	bool alt, shift, ctrl;
//	alt = keyState[Keys::LAlt] || keyState[Keys::RAlt];
//	shift = keyState[Keys::LShift] || keyState[Keys::RShift];
//	ctrl = keyState[Keys::LControl] || keyState[Keys::RControl];
//	KeyInfo keyinfo(pressedKeys.back(), alt, shift, ctrl);  
//}
//
////-----------------------------------//
//
//bool Keyboard::isKeyPressed(Keys::Enum keycode)
//{
//	return keyState[keycode];
//}
//
////-----------------------------------//
//
//void Keyboard::keyPressed(Keys::Enum keycode)
//{
//	keyState[keycode] = true;
//	if(pressedKeys.size > 10)
//		pressedKeys.erase(pressedKeys.begin());
//	pressedKeys.push_back(keycode);
//}
//
////-----------------------------------//
//
//void Keyboard::keyReleased(Keys::Enum keycode)
//{
//	keyState[keycode] = false;
//	for(std::vector<Keys::Enum>::iterator it = pressedKeys.begin(); it != pressedKeys.end(); it++ )
//		{
//			if(*it = keycode) pressedKeys.erase(it);
//		}
//}	

//-----------------------------------//

} } // end namespaces