#include "vapor/input/Keyboard.h"

namespace vapor {
	namespace input {

		KeyInfo::KeyInfo(Keys::Enum keycode, bool alt = false, bool shift = false, bool ctrl = false):
		keycode(keycode), alt(alt), shift(shift), ctrl(ctrl)
		{
		}
		Keys::Enum KeyInfo::getKeyCode()
		{
		return keycode;
		}
		bool KeyInfo::isAltKeyPressed()
		{
		return alt;
		}
		bool KeyInfo::isShiftKeyPressed()
		{
		return shift;
		}
		bool KeyInfo::isCtrlKeyPressed()
		{
		return ctrl;
		}

		Keyboard::Keyboard()
			{
			for(int i = 0; i <= Keys::Pause; i++)
				keyState.push_back(false);
			}
		
		
		Keys::Enum Keyboard::lastKeyPressed()
		{
		return pressedKeys.back();
		}
		
		KeyInfo Keyboard::infoLastKeyPressed()
		{
		bool alt, shift, ctrl;
		alt = keyState[Keys::LAlt] || keyState[Keys::RAlt];
		shift = keyState[Keys::LShift] || keyState[Keys::RShift];
		ctrl = keyState[Keys::LControl] || keyState[Keys::RControl];
		KeyInfo keyinfo(pressedKeys.back(), alt, shift, ctrl);  
		}
		
		bool Keyboard::isKeyPressed(Keys::Enum keycode)
		{
		return keyState[keycode];
		}
		
		void Keyboard::keyPressed(Keys::Enum keycode)
		{
			keyState[keycode] = true;
			if(pressedKeys.size > 10)
				pressedKeys.erase(pressedKeys.begin());
			pressedKeys.push_back(keycode);
		}
		
		void Keyboard::keyReleased(Keys::Enum keycode)
		{
			keyState[keycode] = false;
			for(std::vector<Keys::Enum>::iterator it = pressedKeys.begin(); it != pressedKeys.end(); it++ )
				{
					if(*it = keycode) pressedKeys.erase(it);
				}
		}	
		

	}
}