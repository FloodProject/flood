/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/input/Device.h"

namespace vapor {
	namespace input {

//-----------------------------------//	

/**
 * Represents all the available keyboard keys with a unique key code.
 * When interacting with the Keyboard class you should always use
 * these values (this enumeration was taken from the SFML interface).
 * TODO: How about Japanese keyboard? And Unicode?
 */

namespace Keys
{
	enum Enum
	{
		// Alpha keys
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
		
		// Numeric keys
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
		
		// Special keys
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
		
		// Numeric operations
		Add,          
		Subtract,     
		Multiply,     
		Divide,       
		
		// Arrows
		Left,         
		Right,        
		Up,           
		Down,         
		
		// Numpad numbers
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
		
		// F-keys
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

//-----------------------------------//

/**
 * Different types of keyboard events.
 */

namespace KeyboardEventType
{
	enum Enum
	{
		// Occurs when a keyboard key is pressed
		KeyPressed,
		// Occurs when a keyboard key is released
		KeyReleased
	};
}

//-----------------------------------//

/**
 * Main keyboard event. Holds the related keycode, and whether some 
 * special keys are pressed (handy for testing shortcuts). Finally
 * it also needs to know if it's a key press or release event.
 */

struct KeyEvent : public input::Event
{
	friend class Keyboard;
	
	KeyEvent(Keys::Enum keyCode, 
		bool alt = false, bool shift = false, bool ctrl = false, 
		KeyboardEventType::Enum eventType = KeyboardEventType::KeyPressed);
	
	Keys::Enum keyCode;
	bool altPressed, shiftPressed, ctrlPressed;

private:

	KeyboardEventType::Enum eventType;
};

//-----------------------------------//

/**
 * Keyboard device.
 */

class Keyboard : public Device
{
	//friend class InputManager;

public:	

	Keyboard();
	virtual ~Keyboard();

	Keys::Enum lastKeyPressed();

	KeyEvent infoLastKeyPressed();

	bool isKeyPressed(Keys::Enum keycode);

private:
	
	// Processes an event (only if it's a keyboard event).
	virtual void processEvent( const input::Event& event );

	// Return this device as a keyboard.
	virtual const input::DeviceType::Enum getType();

	void keyPressed(Keys::Enum keycode);
	void keyReleased(Keys::Enum keycode);
	
	std::vector< bool > keyState;
	
	Keys::Enum lastKey;
};

//-----------------------------------//

} } // end namespaces
