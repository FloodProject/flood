/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "EditorInputManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

EditorInputManager::EditorInputManager()
{
	cb += fd::bind( &EditorInputManager::processEvent, this );
}

//-----------------------------------//

void EditorInputManager::processKeyEvent( const wxKeyEvent& event, bool keyDown )
{
	doKeyEvent(cb, event, keyDown);
}

//-----------------------------------//

void EditorInputManager::processMouseEvent( const wxMouseEvent& event )
{
	doMouseEvent(cb, event);	
}

//-----------------------------------//

void EditorInputManager::doKeyEvent( InputProcessCallback cb, const wxKeyEvent& event, bool keyDown )
{
	// Convert from the wxWidgets events to the vaporEngine events.
	KeyEvent ke( 
		convertKeyEnum( event.GetKeyCode() ), 
		event.AltDown(), 
		event.ShiftDown(),
		event.ControlDown(),
		(keyDown) ? KeyboardEventType::KeyPressed : KeyboardEventType::KeyReleased  );

	cb( ke );
}

//-----------------------------------//

void EditorInputManager::doMouseEvent( InputProcessCallback cb, const wxMouseEvent& event )
{
	// Mouse motion
	if( event.Moving() )
	{
		MouseMoveEvent me;
		me.x = event.GetX();
		me.y = event.GetY();
		cb( me );
	}

	// Mouse dragged
	if( event.Dragging() )
	{
		MouseDragEvent me;
		me.x = event.GetX();
		me.y = event.GetY();
		cb( me );
	} 
	
	// Mouse button
	else if( event.IsButton() )
	{
		MouseButton::Enum button;

		switch( event.GetButton() )
		{
		case wxMOUSE_BTN_LEFT:
			button = MouseButton::Left;
			break;
		case wxMOUSE_BTN_RIGHT:
			button = MouseButton::Right;
			break;
		case wxMOUSE_BTN_MIDDLE:
			button = MouseButton::Middle;
			break;
		case wxMOUSE_BTN_AUX1:
			button = MouseButton::Mouse4;
			break;
		case wxMOUSE_BTN_AUX2:
			button = MouseButton::Mouse5;
			break;
		default:
			assert( 0 && "This should not be reachable" );
			button = MouseButton::Middle;
		}

		MouseEventType::Enum type = event.ButtonDown() ?
			MouseEventType::MousePress : MouseEventType::MouseRelease;

		MouseButtonEvent mb(type);
		mb.x = event.GetX();
		mb.y = event.GetY();
		mb.button = button;
		cb( mb );
	}

	else if( event.GetWheelRotation() != 0 )
	{
		// wxWidgets reports a very big value for wheel rotation,
		// so we clamp it down to be uniform with other platforms.
		MouseWheelEvent mwe;
		mwe.delta = ( event.GetWheelRotation() / 120 );
		cb( mwe );
	}

	else if( event.Entering() )
	{
		MouseEvent me( MouseEventType::MouseEnter );
		cb( me );
	}

	else if( event.Leaving() )
	{
		MouseEvent me( MouseEventType::MouseExit );
		cb( me );
	}
}

//-----------------------------------//

Keys::Enum EditorInputManager::convertKeyEnum( int keyCode )
{
	// From the wxWidgets docs:
	// "Note that the range 33 - 126 is reserved for the standard ASCII characters 
	// and that the range 128 - 255 is reserved for the extended ASCII characters 
	// (which are not really standard and thus should be avoid in portable apps!)."

	if( (keyCode >= 'A') && (keyCode <= 'Z') )
		return static_cast<Keys::Enum>( keyCode );
	
	if( (keyCode >= '1') && (keyCode <= '9') )
		return static_cast<Keys::Enum>( keyCode );

	switch( keyCode )
	{
	case WXK_ESCAPE:
		return Keys::Escape;

	case WXK_CONTROL:
		return Keys::LControl;
	case WXK_SHIFT:
		return Keys::LShift;
	case WXK_ALT:
		return Keys::LAlt;
	//case WXK_SYSTEM:
		//return Keys::LSystem;
	//case sf::Key::RControl:
	//	return Keys::RControl;
	//case sf::Key::RShift:
	//	return Keys::RShift;
	//case sf::Key::RAlt:
	//	return Keys::RAlt;
	//case sf::Key::RSystem:
	//	return Keys::RSystem;
	case WXK_MENU:
		return Keys::Menu;

	case WXK_WINDOWS_MENU:
		return Keys::LSuper;
	case WXK_WINDOWS_RIGHT:
		return Keys::RSuper;

	//case sf::Key::LBracket:
	//	return Keys::LBracket;
	//case sf::Key::RBracket:
	//	return Keys::RBracket;
	//case sf::Key::SemiColon:
	//	return Keys::SemiColon;
	//case sf::Key::Comma:
	//	return Keys::Comma;
	//case sf::Key::Period:
	//	return Keys::Period;
	//case sf::Key::Quote:
	//	return Keys::Quote;
	//case sf::Key::Slash:
	//	return Keys::Slash;
	//case sf::Key::BackSlash:
	//	return Keys::BackSlash;
	//case sf::Key::Tilde:
	//	return Keys::Tilde;
	//case sf::Key::Equal:
	//	return Keys::Equal;
	//case WXK_SUBTRACT:
	//	return Keys::Dash;

	case WXK_SPACE:
		return Keys::Space;
	case WXK_RETURN:
		return Keys::Return;
	case WXK_BACK:
		return Keys::Back;
	case WXK_TAB:
		return Keys::Tab;

	case WXK_PAGEUP:
		return Keys::PageUp;
	case WXK_PAGEDOWN:
		return Keys::PageDown;
	case WXK_END:
		return Keys::End;
	case WXK_HOME:
		return Keys::Home;
	case WXK_INSERT:
		return Keys::Insert;
	case WXK_DELETE:
		return Keys::Delete;

	case WXK_NUMPAD_ADD:
		return Keys::Add;
	case WXK_NUMPAD_SUBTRACT:
		return Keys::Subtract;
	case WXK_NUMPAD_MULTIPLY:
		return Keys::Multiply;
	case WXK_NUMPAD_DIVIDE:
		return Keys::Divide;

	case WXK_LEFT:
		return Keys::Left;
	case WXK_RIGHT:
		return Keys::Right;
	case WXK_UP:
		return Keys::Up;
	case WXK_DOWN:
		return Keys::Down;

	case WXK_NUMPAD0:
		return Keys::Numpad0;
	case WXK_NUMPAD1:
		return Keys::Numpad1;
	case WXK_NUMPAD2:
		return Keys::Numpad2;
	case WXK_NUMPAD3:
		return Keys::Numpad3;
	case WXK_NUMPAD4:
		return Keys::Numpad4;
	case WXK_NUMPAD5:
		return Keys::Numpad5;
	case WXK_NUMPAD6:
		return Keys::Numpad6;
	case WXK_NUMPAD7:
		return Keys::Numpad7;
	case WXK_NUMPAD8:
		return Keys::Numpad8;
	case WXK_NUMPAD9:
		return Keys::Numpad9;

	case WXK_F1:
		return Keys::F1;
	case WXK_F2:
		return Keys::F2;
	case WXK_F3:
		return Keys::F3;
	case WXK_F4:
		return Keys::F4;
	case WXK_F5:
		return Keys::F5;
	case WXK_F6:
		return Keys::F6;
	case WXK_F7:
		return Keys::F7;
	case WXK_F8:
		return Keys::F8;
	case WXK_F9:
		return Keys::F9;
	case WXK_F10:
		return Keys::F10;
	case WXK_F11:
		return Keys::F11;
	case WXK_F12:
		return Keys::F12;
	case WXK_F13:
		return Keys::F13;
	case WXK_F14:
		return Keys::F14;
	case WXK_F15:
		return Keys::F15;

	case WXK_PAUSE:
		return Keys::Pause;

	default:
		Log::warn( "Could not map wxWidgets keycode: Invalid keycode" );
		return Keys::Escape;
	}	
}

//-----------------------------------//

} } // end namespaces