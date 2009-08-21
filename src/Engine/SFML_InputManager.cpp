/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_INPUT_SFML

#include "vapor/input/SFML_InputManager.h"

namespace vapor {
	namespace input {

//-----------------------------------//

SFML_InputManager::SFML_InputManager()
{

}

//-----------------------------------//

SFML_InputManager::~SFML_InputManager()
{

}

//-----------------------------------//

void SFML_InputManager::processSFMLEvent( const sf::Event& event )
{
	if( isMouseEvent( event.Type ) )
	{
		processMouseEvent( event );
	}
	else if ( isKeyboardEvent( event.Type ) )
	{
		processKeyboardEvent( event );
	}
	else if ( isJoystickEvent( event.Type ) )
	{
		processJoystickEvent( event );
	}
}

//-----------------------------------//

bool SFML_InputManager::isMouseEvent(sf::Event::EventType eventType)
{
	return eventType == sf::Event::MouseButtonPressed 
		|| eventType == sf::Event::MouseButtonReleased 
		|| eventType == sf::Event::MouseMoved
		|| eventType == sf::Event::MouseWheelMoved 
		|| eventType == sf::Event::MouseEntered 
		|| eventType == sf::Event::MouseLeft;
}

//-----------------------------------//

bool SFML_InputManager::isKeyboardEvent(sf::Event::EventType eventType)
{
	return eventType == sf::Event::KeyPressed 
		|| eventType == sf::Event::KeyReleased;
}

//-----------------------------------//

bool SFML_InputManager::isJoystickEvent(sf::Event::EventType eventType)
{
	return eventType == sf::Event::JoyButtonPressed 
		|| eventType == sf::Event::JoyButtonReleased 
		|| eventType == sf::Event::JoyMoved;
}

//-----------------------------------//

void SFML_InputManager::processMouseEvent(const sf::Event& event)
{
	switch(event.Type)
	{
		case sf::Event::MouseButtonPressed:
		{
			MouseButtonEvent mbe( event.MouseButton.X, event.MouseButton.Y, 
				convertMouseButtonEnum( event.MouseButton.Button ), 
				MouseEventType::MousePress );
			
			processEvent( mbe );
			break;
		}	

		case sf::Event::MouseButtonReleased:
		{
			MouseButtonEvent mbe( event.MouseButton.X, event.MouseButton.Y, 
				convertMouseButtonEnum( event.MouseButton.Button ), 
				MouseEventType::MouseRelease );

			processEvent( mbe );
			break;
		}

		case sf::Event::MouseMoved:
		{
			MouseMoveEvent mme( event.MouseMove.X, event.MouseMove.Y );

			processEvent( mme );
			break;
		}

		case sf::Event::MouseWheelMoved:
		{
			MouseWheelEvent mwe( event.MouseWheel.Delta );
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
		}
	}
}

//-----------------------------------//

void SFML_InputManager::processKeyboardEvent(const sf::Event& event)
{
	switch( event.Type )
	{
		case sf::Event::KeyPressed:
		{
			KeyEvent ke( convertKeyEnum( event.Key.Code ), 
				event.Key.Alt, event.Key.Shift, event.Key.Control,
				KeyboardEventType::KeyPressed );
			processEvent( ke );

			break;
		}
		
		case sf::Event::KeyReleased:
		{
			KeyEvent ke( convertKeyEnum( event.Key.Code ), 
				event.Key.Alt, event.Key.Shift, event.Key.Control,
				KeyboardEventType::KeyReleased );

			processEvent( ke );
			break;
		}
	}
}

//-----------------------------------//

void SFML_InputManager::processJoystickEvent(const sf::Event& event)
{
	switch( event.Type )
	{
		case sf::Event::JoyButtonPressed:
		{
			JoyButtonEvent jbe( event.JoyButton.JoystickId, 
				event.JoyButton.Button, JoystickEventType::JoystickPress );

			processEvent( jbe );
			break;
		}

		case sf::Event::JoyButtonReleased:
		{
			JoyButtonEvent jbe( event.JoyButton.JoystickId, 
				event.JoyButton.Button, JoystickEventType::JoystickRelease );

			processEvent( jbe );
			break;
		}

		case sf::Event::JoyMoved:
		{
			JoyMoveEvent jme( event.JoyMove.JoystickId, 
				convertJoystickAxisEnum( event.JoyMove.Axis ), 
				event.JoyMove.Position );
			
			processEvent( jme ) ;
			break;
		}
	}
}

//-----------------------------------//

MouseButton::Enum SFML_InputManager::convertMouseButtonEnum(
	sf::Mouse::Button mouseButton)
{
	switch(mouseButton)
	{
		case sf::Mouse::Left:
			return MouseButton::Left;

		case sf::Mouse::Right:
			return MouseButton::Right;

		case sf::Mouse::Middle:
			return MouseButton::Middle;

		case sf::Mouse::XButton1:
			return MouseButton::XButton1;

		case sf::Mouse::XButton2:
			return MouseButton::XButton2;

		default:
			warn( "input", 
				"Could not map SFML mouse button: Invalid button" );
			
			return MouseButton::XButton2;
	}
}

//-----------------------------------//

JoystickAxis::Enum SFML_InputManager::convertJoystickAxisEnum(
	sf::Joy::Axis joyAxis)
{
	switch(joyAxis)
	{
		case sf::Joy::AxisX:
			return JoystickAxis::AxisX;

		case sf::Joy::AxisY:
			return JoystickAxis::AxisY;

		case sf::Joy::AxisZ:
			return JoystickAxis::AxisZ;

		case sf::Joy::AxisR:
			return JoystickAxis::AxisR;

		case sf::Joy::AxisU:
			return JoystickAxis::AxisU;

		case sf::Joy::AxisV:
			return JoystickAxis::AxisV;

		case sf::Joy::AxisPOV:
			return JoystickAxis::AxisPOV;

		default:
			warn( "input", "Could not map SFML joystick axis: Invalid axis" );
			return JoystickAxis::AxisPOV;
	}
}

//-----------------------------------//

Keys::Enum SFML_InputManager::convertKeyEnum(sf::Key::Code keycode)
{
	switch(keycode)
	{
		case sf::Key::A:
			return Keys::A;
		case sf::Key::B:
			return Keys::B;
		case sf::Key::C:
			return Keys::C;
		case sf::Key::D:
			return Keys::D;
		case sf::Key::E:
			return Keys::E;
		case sf::Key::F:
			return Keys::F;
		case sf::Key::G:
			return Keys::G;
		case sf::Key::H:
			return Keys::H;
		case sf::Key::I:
			return Keys::I;
		case sf::Key::J:
			return Keys::J;
		case sf::Key::K:
			return Keys::K;
		case sf::Key::L:
			return Keys::L;
		case sf::Key::M:
			return Keys::M;
		case sf::Key::N:
			return Keys::N;
		case sf::Key::O:
			return Keys::O;
		case sf::Key::P:
			return Keys::P;
		case sf::Key::Q:
			return Keys::Q;
		case sf::Key::R:
			return Keys::R;
		case sf::Key::S:
			return Keys::S;
		case sf::Key::T:
			return Keys::T;
		case sf::Key::U:
			return Keys::U;
		case sf::Key::V:
			return Keys::V;
		case sf::Key::W:
			return Keys::W;
		case sf::Key::X:
			return Keys::X;
		case sf::Key::Y:
			return Keys::Y;
		case sf::Key::Z:
			return Keys::Z;

		case sf::Key::Num0:
			return Keys::Num0;
		case sf::Key::Num1:
			return Keys::Num1;
		case sf::Key::Num2:
			return Keys::Num2;
		case sf::Key::Num3:
			return Keys::Num3;
		case sf::Key::Num4:
			return Keys::Num4;
		case sf::Key::Num5:
			return Keys::Num5;
		case sf::Key::Num6:
			return Keys::Num6;
		case sf::Key::Num7:
			return Keys::Num7;
		case sf::Key::Num8:
			return Keys::Num8;
		case sf::Key::Num9:
			return Keys::Num9;

		case sf::Key::Escape:
			return Keys::Escape;

		case sf::Key::LControl:
			return Keys::LControl;
		case sf::Key::LShift:
			return Keys::LShift;
		case sf::Key::LAlt:
			return Keys::LAlt;
		case sf::Key::LSystem:
			return Keys::LSystem;
		case sf::Key::RControl:
			return Keys::RControl;
		case sf::Key::RShift:
			return Keys::RShift;
		case sf::Key::RAlt:
			return Keys::RAlt;
		case sf::Key::RSystem:
			return Keys::RSystem;
		case sf::Key::Menu:
			return Keys::Menu;

		case sf::Key::LBracket:
			return Keys::LBracket;
		case sf::Key::RBracket:
			return Keys::RBracket;
		case sf::Key::SemiColon:
			return Keys::SemiColon;
		case sf::Key::Comma:
			return Keys::Comma;
		case sf::Key::Period:
			return Keys::Period;
		case sf::Key::Quote:
			return Keys::Quote;
		case sf::Key::Slash:
			return Keys::Slash;
		case sf::Key::BackSlash:
			return Keys::BackSlash;
		case sf::Key::Tilde:
			return Keys::Tilde;
		case sf::Key::Equal:
			return Keys::Equal;
		case sf::Key::Dash:
			return Keys::Dash;

		case sf::Key::Space:
			return Keys::Space;
		case sf::Key::Return:
			return Keys::Return;
		case sf::Key::Back:
			return Keys::Back;
		case sf::Key::Tab:
			return Keys::Tab;

		case sf::Key::PageUp:
			return Keys::PageUp;
		case sf::Key::PageDown:
			return Keys::PageDown;
		case sf::Key::End:
			return Keys::End;
		case sf::Key::Home:
			return Keys::Home;
		case sf::Key::Insert:
			return Keys::Insert;
		case sf::Key::Delete:
			return Keys::Delete;

		case sf::Key::Add:
			return Keys::Add;
		case sf::Key::Subtract:
			return Keys::Subtract;
		case sf::Key::Multiply:
			return Keys::Multiply;
		case sf::Key::Divide:
			return Keys::Divide;

		case sf::Key::Left:
			return Keys::Left;
		case sf::Key::Right:
			return Keys::Right;
		case sf::Key::Up:
			return Keys::Up;
		case sf::Key::Down:
			return Keys::Down;

		case sf::Key::Numpad0:
			return Keys::Numpad0;
		case sf::Key::Numpad1:
			return Keys::Numpad1;
		case sf::Key::Numpad2:
			return Keys::Numpad2;
		case sf::Key::Numpad3:
			return Keys::Numpad3;
		case sf::Key::Numpad4:
			return Keys::Numpad4;
		case sf::Key::Numpad5:
			return Keys::Numpad5;
		case sf::Key::Numpad6:
			return Keys::Numpad6;
		case sf::Key::Numpad7:
			return Keys::Numpad7;
		case sf::Key::Numpad8:
			return Keys::Numpad8;
		case sf::Key::Numpad9:
			return Keys::Numpad9;

		case sf::Key::F1:
			return Keys::F1;
		case sf::Key::F2:
			return Keys::F2;
		case sf::Key::F3:
			return Keys::F3;
		case sf::Key::F4:
			return Keys::F4;
		case sf::Key::F5:
			return Keys::F5;
		case sf::Key::F6:
			return Keys::F6;
		case sf::Key::F7:
			return Keys::F7;
		case sf::Key::F8:
			return Keys::F8;
		case sf::Key::F9:
			return Keys::F9;
		case sf::Key::F10:
			return Keys::F10;
		case sf::Key::F11:
			return Keys::F11;
		case sf::Key::F12:
			return Keys::F12;
		case sf::Key::F13:
			return Keys::F13;
		case sf::Key::F14:
			return Keys::F14;
		case sf::Key::F15:
			return Keys::F15;

		case sf::Key::Pause:
			return Keys::Pause;

		default:
			warn( "input", "Could not map SFML keycode: Invalid keycode" );
			return Keys::Escape;
	}
}

//-----------------------------------//

} } // end namespaces

#endif
