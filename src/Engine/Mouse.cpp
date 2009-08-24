/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/Mouse.h"

namespace vapor {
	namespace input {

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
		
		case MouseButton::XButton1:
			return mouseInfo.XButton1;
			break;
		
		case MouseButton::XButton2:
			return mouseInfo.XButton2;
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

void Mouse::mouseButtonPressed(const MouseButtonEvent& press)
{
	mouseInfo.x = press.x;
	mouseInfo.y = press.y;
	
	switch(press.button)
	{
		case MouseButton::Left:
			mouseInfo.leftButton = true;
			break;
		
		case MouseButton::Right:
			mouseInfo.rightButton = true;
			break;
		
		case MouseButton::Middle:
			mouseInfo.middleButton = true;
			break;
		
		case MouseButton::XButton1:
			mouseInfo.XButton1 = true;
			break;
		
		case MouseButton::XButton2:
			mouseInfo.XButton2 = true;
			break;
	}

	if ( !onMouseButtonPress.empty() )
	{
		onMouseButtonPress( press );
	}
}

//-----------------------------------//

void Mouse::mouseButtonReleased(const MouseButtonEvent& mbe)
{
	mouseInfo.x = mbe.x;
	mouseInfo.y = mbe.y;
	
	switch( mbe.button )
	{
		case MouseButton::Left:
			mouseInfo.leftButton = false;
			break;
		
		case MouseButton::Right:
			mouseInfo.rightButton = false;
			break;
		
		case MouseButton::Middle:
			mouseInfo.middleButton = false;
			break;
		
		case MouseButton::XButton1:
			mouseInfo.XButton1 = false;
			break;
		
		case MouseButton::XButton2:
			mouseInfo.XButton2 = false;
			break;
	}

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

MouseInfo::MouseInfo()
	: x(0), y(0), leftButton(false), rightButton(false), middleButton(false),
	XButton1(false), XButton2(false), insideWindow(false)
{
 
}

//-----------------------------------//

MouseEvent::MouseEvent( MouseEventType::Enum eventType )
	: Event( DeviceType::Mouse ), eventType( eventType )
{

}

//-----------------------------------//

MouseMoveEvent::MouseMoveEvent( int x, int y )
	: MouseEvent( MouseEventType::MouseMove ), x( x ), y( y ) 
{

}

//-----------------------------------//

MouseButtonEvent::MouseButtonEvent(int x, int y, 
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