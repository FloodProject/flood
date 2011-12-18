/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "SFML_Input.h"

#include "Input/InputManager.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/Joystick.h"

#ifdef ENABLE_INPUT_SFML

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static bool isMouseEvent(sf::Event::EventType eventType)
{
	return eventType == sf::Event::MouseButtonPressed 
		|| eventType == sf::Event::MouseButtonReleased 
		|| eventType == sf::Event::MouseMoved
		|| eventType == sf::Event::MouseWheelMoved 
		|| eventType == sf::Event::MouseEntered 
		|| eventType == sf::Event::MouseLeft;
}

//-----------------------------------//

static bool isKeyboardEvent(sf::Event::EventType eventType)
{
	return eventType == sf::Event::KeyPressed 
		|| eventType == sf::Event::KeyReleased
		|| eventType == sf::Event::TextEntered;
}

//-----------------------------------//

static bool isJoystickEvent(sf::Event::EventType eventType)
{
	return eventType == sf::Event::JoystickButtonPressed 
		|| eventType == sf::Event::JoystickButtonReleased 
		|| eventType == sf::Event::JoystickConnected
		|| eventType == sf::Event::JoystickDisconnected
		|| eventType == sf::Event::JoystickMoved;
}

//-----------------------------------//

MouseButton::Enum ConvertMouseButtonEnum(sf::Mouse::Button mouseButton)
{
	switch(mouseButton)
	{
	case sf::Mouse::Left: return MouseButton::Left;
	case sf::Mouse::Right: return MouseButton::Right;
	case sf::Mouse::Middle: return MouseButton::Middle;
	case sf::Mouse::XButton1: return MouseButton::Mouse4;
	case sf::Mouse::XButton2: return MouseButton::Mouse5;
	default:
		LogWarn( "Could not map SFML mouse button: Invalid button" );
		return MouseButton::Mouse5;
	}
}

//-----------------------------------//

static JoystickAxis::Enum ConvertJoystickAxisEnum(sf::Joystick::Axis axis)
{
	switch(axis)
	{
	case sf::Joystick::X: return JoystickAxis::AxisX;
	case sf::Joystick::Y: return JoystickAxis::AxisY;
	case sf::Joystick::Z: return JoystickAxis::AxisZ;
	case sf::Joystick::R: return JoystickAxis::AxisR;
	case sf::Joystick::U: return JoystickAxis::AxisU;
	case sf::Joystick::V: return JoystickAxis::AxisV;
	case sf::Joystick::PovX: return JoystickAxis::AxisPOV;
	case sf::Joystick::PovY: return JoystickAxis::AxisPOV;
	default:
		LogWarn( "Could not map SFML joystick axis: Invalid axis" );
		return JoystickAxis::AxisPOV;
	}
}

//-----------------------------------//

static Keys::Enum ConvertKeyEnum(sf::Keyboard::Key keycode)
{
	switch(keycode)
	{
	case sf::Keyboard::A: return Keys::A;
	case sf::Keyboard::B: return Keys::B;
	case sf::Keyboard::C: return Keys::C;
	case sf::Keyboard::D: return Keys::D;
	case sf::Keyboard::E: return Keys::E;
	case sf::Keyboard::F: return Keys::F;
	case sf::Keyboard::G: return Keys::G;
	case sf::Keyboard::H: return Keys::H;
	case sf::Keyboard::I: return Keys::I;
	case sf::Keyboard::J: return Keys::J;
	case sf::Keyboard::K: return Keys::K;
	case sf::Keyboard::L: return Keys::L;
	case sf::Keyboard::M: return Keys::M;
	case sf::Keyboard::N: return Keys::N;
	case sf::Keyboard::O: return Keys::O;
	case sf::Keyboard::P: return Keys::P;
	case sf::Keyboard::Q: return Keys::Q;
	case sf::Keyboard::R: return Keys::R;
	case sf::Keyboard::S: return Keys::S;
	case sf::Keyboard::T: return Keys::T;
	case sf::Keyboard::U: return Keys::U;
	case sf::Keyboard::V: return Keys::V;
	case sf::Keyboard::W: return Keys::W;
	case sf::Keyboard::X: return Keys::X;
	case sf::Keyboard::Y: return Keys::Y;
	case sf::Keyboard::Z: return Keys::Z;

	case sf::Keyboard::Num0: return Keys::Num0;
	case sf::Keyboard::Num1: return Keys::Num1;
	case sf::Keyboard::Num2: return Keys::Num2;
	case sf::Keyboard::Num3: return Keys::Num3;
	case sf::Keyboard::Num4: return Keys::Num4;
	case sf::Keyboard::Num5: return Keys::Num5;
	case sf::Keyboard::Num6: return Keys::Num6;
	case sf::Keyboard::Num7: return Keys::Num7;
	case sf::Keyboard::Num8: return Keys::Num8;
	case sf::Keyboard::Num9: return Keys::Num9;

	case sf::Keyboard::Escape:   return Keys::Escape;
	case sf::Keyboard::LControl: return Keys::LControl;
	case sf::Keyboard::LShift:   return Keys::LShift;
	case sf::Keyboard::LAlt:     return Keys::LAlt;
	case sf::Keyboard::LSystem:  return Keys::LSuper;
	case sf::Keyboard::RControl: return Keys::RControl;
	case sf::Keyboard::RShift:   return Keys::RShift;
	case sf::Keyboard::RAlt:     return Keys::RAlt;
	case sf::Keyboard::RSystem:  return Keys::RSuper;
	case sf::Keyboard::Menu:     return Keys::Menu;

	case sf::Keyboard::LBracket:  return Keys::LBracket;
	case sf::Keyboard::RBracket:  return Keys::RBracket;
	case sf::Keyboard::SemiColon: return Keys::SemiColon;
	case sf::Keyboard::Comma:     return Keys::Comma;
	case sf::Keyboard::Period:    return Keys::Period;
	case sf::Keyboard::Quote:     return Keys::Quote;
	case sf::Keyboard::Slash:     return Keys::Slash;
	case sf::Keyboard::BackSlash: return Keys::BackSlash;
	case sf::Keyboard::Tilde:     return Keys::Tilde;
	case sf::Keyboard::Equal:     return Keys::Equal;
	case sf::Keyboard::Dash:      return Keys::Dash;

	case sf::Keyboard::Space:  return Keys::Space;
	case sf::Keyboard::Return: return Keys::Return;
	case sf::Keyboard::Back:   return Keys::Back;
	case sf::Keyboard::Tab:    return Keys::Tab;

	case sf::Keyboard::PageUp:   return Keys::PageUp;
	case sf::Keyboard::PageDown: return Keys::PageDown;
	case sf::Keyboard::End:      return Keys::End;
	case sf::Keyboard::Home:     return Keys::Home;
	case sf::Keyboard::Insert:   return Keys::Insert;
	case sf::Keyboard::Delete:   return Keys::Delete;

	case sf::Keyboard::Add:      return Keys::Add;
	case sf::Keyboard::Subtract: return Keys::Subtract;
	case sf::Keyboard::Multiply: return Keys::Multiply;
	case sf::Keyboard::Divide:   return Keys::Divide;

	case sf::Keyboard::Left:  return Keys::Left;
	case sf::Keyboard::Right: return Keys::Right;
	case sf::Keyboard::Up:    return Keys::Up;
	case sf::Keyboard::Down:  return Keys::Down;

	case sf::Keyboard::Numpad0: return Keys::Numpad0;
	case sf::Keyboard::Numpad1: return Keys::Numpad1;
	case sf::Keyboard::Numpad2: return Keys::Numpad2;
	case sf::Keyboard::Numpad3: return Keys::Numpad3;
	case sf::Keyboard::Numpad4: return Keys::Numpad4;
	case sf::Keyboard::Numpad5: return Keys::Numpad5;
	case sf::Keyboard::Numpad6: return Keys::Numpad6;
	case sf::Keyboard::Numpad7: return Keys::Numpad7;
	case sf::Keyboard::Numpad8: return Keys::Numpad8;
	case sf::Keyboard::Numpad9: return Keys::Numpad9;

	case sf::Keyboard::F1: return Keys::F1;
	case sf::Keyboard::F2: return Keys::F2;
	case sf::Keyboard::F3: return Keys::F3;
	case sf::Keyboard::F4: return Keys::F4;
	case sf::Keyboard::F5: return Keys::F5;
	case sf::Keyboard::F6: return Keys::F6;
	case sf::Keyboard::F7: return Keys::F7;
	case sf::Keyboard::F8: return Keys::F8;
	case sf::Keyboard::F9: return Keys::F9;
	case sf::Keyboard::F10: return Keys::F10;
	case sf::Keyboard::F11: return Keys::F11;
	case sf::Keyboard::F12: return Keys::F12;
	case sf::Keyboard::F13: return Keys::F13;
	case sf::Keyboard::F14: return Keys::F14;
	case sf::Keyboard::F15: return Keys::F15;

	case sf::Keyboard::Pause: return Keys::Pause;

	default:
		LogWarn( "Could not map SFML keycode: Invalid keycode" );
		return Keys::Escape;
	}
}

//-----------------------------------//

SFML_Input::SFML_Input()
{
}

//-----------------------------------//

void SFML_Input::processSFMLEvent( const sf::Event& event )
{
	if( isMouseEvent(event.Type) )
	{
		processMouseEvent(event);
	}
	else if ( isKeyboardEvent(event.Type) )
	{
		processKeyboardEvent(event);
	}
	else if ( isJoystickEvent(event.Type) )
	{
		processJoystickEvent(event);
	}
}

//-----------------------------------//

void SFML_Input::processMouseEvent(const sf::Event& event)
{
	short mbX = static_cast<short>(event.MouseButton.X); 
	short mbY = static_cast<short>(event.MouseButton.Y);

	switch(event.Type)
	{
	case sf::Event::MouseButtonPressed:
	{
		MouseButtonEvent mbe( MouseEventType::MousePress );
		
		mbe.x = mbX;
		mbe.y = mbY;
		mbe.button = ConvertMouseButtonEnum( event.MouseButton.Button );
		
		processEvent( mbe );
		break;
	}	

	case sf::Event::MouseButtonReleased:
	{
		MouseButtonEvent mbe( MouseEventType::MouseRelease );
		
		mbe.x = mbX;
		mbe.y = mbY;
		mbe.button = ConvertMouseButtonEnum( event.MouseButton.Button );

		processEvent( mbe );
		break;
	}

	case sf::Event::MouseMoved:
	{
		short mmX = static_cast<short>(event.MouseMove.X);
		short mmY = static_cast<short>(event.MouseMove.Y);
		
		MouseMoveEvent mme;
		mme.x = mmX;
		mme.y = mmY;

		processEvent( mme );
		break;
	}

	case sf::Event::MouseWheelMoved:
	{
		MouseWheelEvent mwe;
		mwe.delta = short(event.MouseWheel.Delta);
		processEvent( mwe );

		break;
	}

	case sf::Event::MouseEntered:
	{
		MouseEvent me( MouseEventType::MouseEnter );
		processEvent( me );

		break;
	}

	case sf::Event::MouseLeft:
	{
		MouseEvent me( MouseEventType::MouseExit );
		processEvent( me );

		break;
	} }
}

//-----------------------------------//

void SFML_Input::processKeyboardEvent(const sf::Event& event)
{
	switch( event.Type )
	{
	case sf::Event::KeyPressed:
	{
		KeyEvent ke(KeyboardEventType::KeyPressed);
		ke.keyCode = ConvertKeyEnum( event.Key.Code );
		ke.altPressed = event.Key.Alt;
		ke.shiftPressed = event.Key.Shift;
		ke.ctrlPressed = event.Key.Control;
		processEvent( ke );
		break;
	}
	
	case sf::Event::KeyReleased:
	{
		KeyEvent ke(KeyboardEventType::KeyReleased);
		ke.keyCode = ConvertKeyEnum( event.Key.Code );
		ke.altPressed = event.Key.Alt;
		ke.shiftPressed = event.Key.Shift;
		ke.ctrlPressed = event.Key.Control;
		processEvent( ke );
		break;
	}

	case sf::Event::TextEntered:
	{
		KeyEvent ke(KeyboardEventType::KeyText);
		ke.unicode = event.Text.Unicode;
		processEvent(ke);
		break;
	} }
}

//-----------------------------------//

void SFML_Input::processJoystickEvent(const sf::Event& event)
{
	switch( event.Type )
	{
#if 0
	case sf::Event::JoystickButtonPressed:
	{
		JoyButtonEvent jbe( event.JoyButton.JoystickId, 
		event.JoyButton.Button, JoystickEventType::JoystickPress );

		processEvent( jbe );
		break;
	}

	case sf::Event::JoystickButtonReleased:
	{
		JoyButtonEvent jbe( event.JoyButton.JoystickId, 
		event.JoyButton.Button, JoystickEventType::JoystickRelease );

		processEvent( jbe );
		break;
	}

	case sf::Event::JoystickMoved:
	{
		JoyMoveEvent jme( event.JoyMove.JoystickId, 
		ConvertJoystickAxisEnum( event.JoyMove.Axis ), 
		event.JoyMove.Position );
		
		processEvent( jme ) ;
		break;
	}
#endif
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif