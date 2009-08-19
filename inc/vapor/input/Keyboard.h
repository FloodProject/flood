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
	namespace input {
		namespace Keys{
			enum Enum
				{
				A = 'a',
				B = 'b',
				C = 'c',
				D = 'd',
				E = 'e',
				F = 'f',
				G = 'g',
				H = 'h',
				I = 'i',
				J = 'j',
				K = 'k',
				L = 'l',
				M = 'm',
				N = 'n',
				O = 'o',
				P = 'p',
				Q = 'q',
				R = 'r',
				S = 's',
				T = 't',
				U = 'u',
				V = 'v',
				W = 'w',
				X = 'x',
				Y = 'y',
				Z = 'z',
				Num0 = '0',
				Num1 = '1',
				Num2 = '2',
				Num3 = '3',
				Num4 = '4',
				Num5 = '5',
				Num6 = '6',
				Num7 = '7',
				Num8 = '8',
				Num9 = '9', 
				Escape = 256,
				LControl,
				LShift,
				LAlt,
				LSystem,      
				RControl,
				RShift,
				RAlt,
				RSystem,      
				Menu,
				LBracket,     
				RBracket,     
				SemiColon,    
				Comma,        
				Period,       
				Quote,        
				Slash,        
				BackSlash,
				Tilde,        
				Equal,        
				Dash,         
				Space,
				Return,
				Back,
				Tab,
				PageUp,
				PageDown,
				End,
				Home,
				Insert,
				Delete,
				Add,          
				Subtract,     
				Multiply,     
				Divide,       
				Left,         
				Right,        
				Up,           
				Down,         
				Numpad0,
				Numpad1,
				Numpad2,
				Numpad3,
				Numpad4,
				Numpad5,
				Numpad6,
				Numpad7,
				Numpad8,
				Numpad9,
				F1,
				F2,
				F3,
				F4,
				F5,
				F6,
				F7,
				F8,
				F9,
				F10,
				F11,
				F12,
				F13,
				F14,
				F15,
				Pause,
				};
		
			}

class KeyInfo
{
public:
	KeyInfo(Keys::Enum keycode, bool alt = false, bool shift = false, bool ctrl = false);
	Keys::Enum getKeyCode();
	bool isAltKeyPressed();
	bool isShiftKeyPressed();
	bool isCtrlKeyPressed();

private:
	Keys::Enum keycode;
	bool alt, shift, ctrl;
}

class Keyboard : public Device
{
friend class InputManager;
public:	
	Keyboard();
	std::vector<Keys::Enum> getKeysPressed();
	Keys::Enum lastKeyPressed();
	KeyInfo infoLastKeyPressed();
	bool isKeyPressed(Keys::Enum keycode);

private:
	void keyPressed(Keys::Enum keycode);
	void keyReleased(Keys::Enum keycode);
	std::vector<bool> keyState;
	std::vector<Keys::Enum> pressedKeys;

}


}}
