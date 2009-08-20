/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/input/Mouse.h"
#include "vapor/input/Type.h"

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

	const MouseEvent& mevt = static_cast< const MouseEvent& > ( event );
	switch(mevt.eventType)
		{
		case MouseEventType::MousePress:
			mouseButtonPressed(mevt);
			break;

		case MouseEventType::MouseRelease:
			mouseButtonReleased(mevt);
			break;

		case MouseEventType::MouseMove:
			mouseMoved(mevt);
			break;

		case MouseEventType::MouseEnter:
			mouseEnter();
			break;

		case MouseEventType::MouseExit:
			mouseExit();
			break;

		case MouseEventType::MouseWheelMove:
			mouseWheelMove(mevt);
			break;
		}
}
//-----------------------------------//
MouseInfo Mouse::getMouseInfo()
{
	return mouseinfo;
}
	
//-----------------------------------//

void Mouse::mouseButtonPressed(const MouseEvent& mevt)
{
	
	const MouseButtonEvent& press = static_cast< const MouseButtonEvent& > ( mevt );
	mouseinfo.x = press.x;
	mouseinfo.y = press.y;
	switch(press.button)
		{
		case MouseButton::Left:
			mouseinfo.leftButton = true;
			break;
		case MouseButton::Right:
			mouseinfo.rightButton = true;
			break;
		case MouseButton::Middle:
			mouseinfo.middleButton = true;
			break;
		case MouseButton::XButton1:
			mouseinfo.XButton1 = true;
			break;
		case MouseButton::XButton2:
			mouseinfo.XButton2 = true;
			break;

		}
	
}

//-----------------------------------//

void Mouse::mouseButtonReleased(const MouseEvent& mevt)
{
	const MouseButtonEvent& release = static_cast< const MouseButtonEvent& > ( mevt );
	mouseinfo.x = release.x;
	mouseinfo.y = release.y;
	switch(release.button)
		{
		case MouseButton::Left:
			mouseinfo.leftButton = false;
			break;
		case MouseButton::Right:
			mouseinfo.rightButton = false;
			break;
		case MouseButton::Middle:
			mouseinfo.middleButton = false;
			break;
		case MouseButton::XButton1:
			mouseinfo.XButton1 = false;
			break;
		case MouseButton::XButton2:
			mouseinfo.XButton2 = false;
			break;

		}
	
}

//-----------------------------------//

void Mouse::mouseMoved(const MouseEvent& mevt)
{	
	const MouseMoveEvent& move = static_cast< const MouseMoveEvent& > ( mevt );
	mouseinfo.x = move.x;
	mouseinfo.y = move.y;
}

//-----------------------------------//

void Mouse::mouseEnter()
{
	mouseinfo.inWindow = true;
}

//-----------------------------------//

void Mouse::mouseExit()
{
	mouseinfo.inWindow = false;
}

//-----------------------------------//

void Mouse::mouseWheelMove(const MouseEvent& mevt)
{
	const MouseWheelEvent& mwevt = static_cast< const MouseWheelEvent& > ( mevt );
	
}

//-----------------------------------//

	}
} // end namespaces