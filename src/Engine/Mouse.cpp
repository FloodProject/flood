/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/input/Mouse.h"

namespace vapor {

//-----------------------------------//

#define CAST_EVENT(type, var, evt)		\
	type var = static_cast<type>(evt);

void Mouse::processEvent( const Event& event )
{
	if( event.deviceType != DeviceType::Mouse )
		return;

	CAST_EVENT(const MouseEvent&, mevt, event)
	
	switch( mevt.eventType )
	{
		case MouseEventType::MousePress:
		{
			CAST_EVENT(const MouseButtonEvent&, mbe, mevt)
			mouseButtonPressed( mbe );
			break;
		}
		
		case MouseEventType::MouseRelease:
		{
			CAST_EVENT(const MouseButtonEvent&, mbe, mevt)
			mouseButtonReleased( mbe );
			break;
		}
		
		case MouseEventType::MouseMove:
		{
			CAST_EVENT(const MouseMoveEvent&, mm, mevt)
			mouseMoved( mm );
			break;
		}

		case MouseEventType::MouseDrag:
		{
			CAST_EVENT(const MouseDragEvent&, mde, mevt)			
			mouseDragged( mde );
			break;
		}

		case MouseEventType::MouseWheelMove:
		{
			CAST_EVENT(const MouseWheelEvent&, mwe, mevt)
			mouseWheelMove( mwe );
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
	}
}

//-----------------------------------//

bool Mouse::isButtonPressed( MouseButton::Enum button ) const
{
	const MouseInfo& mouseInfo = getMouseInfo();

	switch( button )
	{
		case MouseButton::Left:
			return mouseInfo.leftButton;
		case MouseButton::Right:
			return mouseInfo.rightButton;
		case MouseButton::Middle:
			return mouseInfo.middleButton;
		case MouseButton::Mouse4:
			return mouseInfo.Mouse4;
		case MouseButton::Mouse5:
			return mouseInfo.Mouse5;
	}

	warn( "input", "Could not map enum: Invalid mouse button" );
	return MouseButton::Left;
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

void Mouse::mouseDragged(const MouseDragEvent& mde)
{
	MouseDragEvent& me = const_cast<MouseDragEvent&>(mde);

	me.dx = mouseInfo.x - mde.x;
	me.dy = mouseInfo.y - mde.y;

	mouseInfo.x = mde.x;
	mouseInfo.y = mde.y;

	if ( !onMouseDrag.empty() )
	{
		onMouseDrag( mde );
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
	: x(0), y(0), 
	leftButton(false), rightButton(false), middleButton(false),
	Mouse4(false), Mouse5(false), insideWindow(false)
{ }

//-----------------------------------//

MouseEvent::MouseEvent( MouseEventType::Enum eventType )
	: Event( DeviceType::Mouse ), eventType( eventType )
{ }

//-----------------------------------//

MouseMoveEvent::MouseMoveEvent()
	: MouseEvent( MouseEventType::MouseMove )
{ }

//-----------------------------------//

MouseDragEvent::MouseDragEvent()
	: MouseEvent( MouseEventType::MouseDrag )
{ }

//-----------------------------------//

MouseButtonEvent::MouseButtonEvent( MouseEventType::Enum eventType )
	: MouseEvent(eventType)
{ }

//-----------------------------------//

MouseWheelEvent::MouseWheelEvent()
	: MouseEvent( MouseEventType::MouseWheelMove )
{ }

//-----------------------------------//

} // end namespace
