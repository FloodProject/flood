/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/Mouse.h"

namespace vapor { namespace input {

//-----------------------------------//

Mouse::Mouse()
{

}

//-----------------------------------//

Mouse::~Mouse()
{

}

//-----------------------------------//

const input::DeviceType::Enum Mouse::getType()
{
	return input::DeviceType::Mouse; 
}

//-----------------------------------//

void Mouse::processEvent( const input::Event& event )
{
	if( event.deviceType != DeviceType::Mouse )
	{
		return;
	}

	const MouseEvent& mevt = 
		static_cast< const MouseEvent& > ( event );
	
	switch( mevt.eventType )
	{
		case MouseEventType::MousePress:
		{
			const MouseButtonEvent& mbe = 
				static_cast< const MouseButtonEvent& > ( mevt );
			mouseButtonPressed( mbe );
			break;
		}
		
		case MouseEventType::MouseRelease:
		{
			const MouseButtonEvent& mbe = 
				static_cast< const MouseButtonEvent& > ( mevt );
			mouseButtonReleased( mbe );
			break;
		}
		
		case MouseEventType::MouseMove:
		{
			const MouseMoveEvent& mm = 
				static_cast< const MouseMoveEvent& > ( mevt );
			mouseMoved( mm );
			break;
		}
		
		case MouseEventType::MouseEnter:
		{
			mouseEnter();
			break;
		}
		
		case MouseEventType::MouseExit:
		{
			mouseExit();
			break;
		}
		
		case MouseEventType::MouseWheelMove:
		{
			const MouseWheelEvent& mwe = 
				static_cast< const MouseWheelEvent& > ( mevt );
			mouseWheelMove( mwe );
			break;
		}
	}
}

//-----------------------------------//

bool Mouse::isButtonPressed( MouseButton::Enum button )
{
	const MouseInfo& mouseInfo = getMouseInfo();

	switch( button )
	{
		case MouseButton::Left:
			return mouseInfo.leftButton;
			break;
		
		case MouseButton::Right:
			return mouseInfo.rightButton;
			break;
		
		case MouseButton::Middle:
			return mouseInfo.middleButton;
			break;
		
		case MouseButton::Mouse4:
			return mouseInfo.Mouse4;
			break;
		
		case MouseButton::Mouse5:
			return mouseInfo.Mouse5;
			break;
	}

	warn( "input", "Could  not map enum: Invalid mouse button" );

	return MouseButton::Left;
}

//-----------------------------------//

const MouseInfo& Mouse::getMouseInfo()
{
	return mouseInfo;
}
	
//-----------------------------------//

void Mouse::mouseMoved(const MouseMoveEvent& mme)
{	
	mouseInfo.x = mme.x;
	mouseInfo.y = mme.y;

	if ( !onMouseMove.empty() )
	{
		onMouseMove( mme );
	}
}

//-----------------------------------//

void Mouse::mouseButtonPressed(const MouseButtonEvent& mbp)
{
	mouseInfo.x = mbp.x;
	mouseInfo.y = mbp.y;
	
	setMouseButton( mbp.button, true );

	if ( !onMouseButtonPress.empty() )
	{
		onMouseButtonPress( mbp );
	}
}

//-----------------------------------//

void Mouse::mouseButtonReleased(const MouseButtonEvent& mbe)
{
	mouseInfo.x = mbe.x;
	mouseInfo.y = mbe.y;
	
	setMouseButton( mbe.button, false );

	if ( !onMouseButtonRelease.empty() )
	{
		onMouseButtonRelease( mbe );
	}
}

//-----------------------------------//

void Mouse::mouseEnter()
{
	mouseInfo.insideWindow = true;

	if ( !onMouseEnter.empty() )
	{
		onMouseEnter( );
	}
}

//-----------------------------------//

void Mouse::mouseExit()
{
	mouseInfo.insideWindow = false;

	if ( !onMouseExit.empty() )
	{
		onMouseExit( );
	}
}

//-----------------------------------//

void Mouse::mouseWheelMove(const MouseWheelEvent& mevt)
{
	if ( !onMouseWheelMove.empty() )
	{
		onMouseWheelMove( mevt );
	}	
}

//-----------------------------------//

void Mouse::setMouseButton( MouseButton::Enum btn, bool state )
{
	switch( btn )
	{
		case MouseButton::Left:
			mouseInfo.leftButton = state;
			break;
		
		case MouseButton::Right:
			mouseInfo.rightButton = state;
			break;
		
		case MouseButton::Middle:
			mouseInfo.middleButton = state;
			break;
		
		case MouseButton::Mouse4:
			mouseInfo.Mouse4 = state;
			break;
		
		case MouseButton::Mouse5:
			mouseInfo.Mouse5 = state;
			break;
	}
}

//-----------------------------------//

MouseInfo::MouseInfo()
	: x(0), y(0), leftButton(false), rightButton(false), middleButton(false),
	Mouse4(false), Mouse5(false), insideWindow(false)
{
 
}

//-----------------------------------//

MouseEvent::MouseEvent( MouseEventType::Enum eventType )
	: Event( DeviceType::Mouse ), eventType( eventType )
{

}

//-----------------------------------//

MouseMoveEvent::MouseMoveEvent( short x, short y )
	: MouseEvent( MouseEventType::MouseMove ), x( x ), y( y ) 
{

}

//-----------------------------------//

MouseButtonEvent::MouseButtonEvent(short x, short y, 
	MouseButton::Enum button, MouseEventType::Enum eventType)
	: MouseEvent(eventType), x(x), y(y), button(button) 
{

}

//-----------------------------------//

MouseWheelEvent::MouseWheelEvent( int delta )
	: MouseEvent( MouseEventType::MouseWheelMove ), delta( delta )
{

}

//-----------------------------------//

} } // end namespaces
