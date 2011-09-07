/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Core/Event.h"
#include "Controllers/FirstPersonController.h"
#include "Math/Helpers.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Render/Device.h"
#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(FirstPersonController, CameraController)
REFLECT_CLASS_END()

//-----------------------------------//

static const float DEFAULT_LIMIT_XAXIS = 89.0f;

FirstPersonController::FirstPersonController()
	: clampMovementX( true )
	, mouseWheel(0)
{
	registerCallbacks();
}

//-----------------------------------//

FirstPersonController::~FirstPersonController()
{
	//window->onWindowFocusChange.Disconnect( this, &FirstPersonController::onWindowFocusChange );

	InputManager* input = GetInputManager();
	if( !input ) return;

	Keyboard* keyboard = input->getKeyboard();
	keyboard->onKeyPress.Disconnect( this, &FirstPersonController::onKeyPressed );
	
	Mouse* mouse = input->getMouse();
	mouse->onMouseMove.Disconnect( this, &FirstPersonController::onMouseMove );
	mouse->onMouseDrag.Disconnect( this, &FirstPersonController::onMouseDrag );
	mouse->onMouseWheelMove.Disconnect( this, &FirstPersonController::onMouseWheel );
}

//-----------------------------------//

void FirstPersonController::setEnabled(bool enabled)
{
	CameraController::setEnabled(enabled);
	
	Window* window = GetInputManager()->getWindow();
	if( !window ) return;

	if(enabled)
	{
		oldMousePosition = window->getCursorPosition();
		centerCursor();
	}
	else
	{
		window->setCursorPosition(oldMousePosition);
		relativePosition.zero();
	}
}

//-----------------------------------//

void FirstPersonController::_update( float delta )
{
	InputManager* input = GetInputManager();
	if( !input ) return;

	Window* window = input->getWindow();
	if( !window ) return;

	if( window->hasFocus() )
		checkControls( delta );
}

//-----------------------------------//

void FirstPersonController::checkControls( float delta )
{
	const TransformPtr& transform = getEntity()->getTransform();
	Vector3 position = transform->getPosition();
	
	Vector3 moveVector;
	bool viewChanged = false;
	
	// Check mouse movement.
	if( mouseDistance != Vector2i(0, 0) )
	{
		Vector3 rotate( mouseDistance.y, -mouseDistance.x, 0 );
		rotate *= delta * lookSensivity;

		rotation += rotate;

		// Restrict X-axis movement by some deegres.
		float& xang = rotation.x;
		MathClamp( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

		mouseDistance.zero();
		viewChanged = true;
	}

	// Check mouse wheel movement.
	if( mouseWheel != 0 )
	{
		moveVector += Vector3::UnitZ * mouseWheel * 100;
		mouseWheel = 0;
	}

	// Check keyboard movement.
	Keyboard* keyboard = GetInputManager()->getKeyboard();
	const std::vector< bool >& state = keyboard->getKeyState();

	if( state[Keys::W] )
		moveVector +=  Vector3::UnitZ;

	if( state[Keys::S] )
		moveVector += -Vector3::UnitZ;
	
	if( state[Keys::A] )
		moveVector +=  Vector3::UnitX;

	if( state[Keys::D] )
		moveVector += -Vector3::UnitX;
	
	if( state[Keys::Q] )
		moveVector +=  Vector3::UnitY;

	if( state[Keys::Z] )
		moveVector += -Vector3::UnitY;

	if( moveVector != Vector3::Zero )
	{
		moveVector *= delta * moveSensivity;
		relativePosition += Matrix4x3::createRotation(rotation) * moveVector;
		viewChanged = true;
	}

	// TODO: hack.
	static const float epsilon = 0.05f;

	if( (fabs(relativePosition.x) < epsilon)
		&& (fabs(relativePosition.y) < epsilon)
		&& (fabs(relativePosition.z) < epsilon) )
	{
		relativePosition.zero();
	}

	if( viewChanged || (relativePosition != Vector3::Zero) )
	{
		Vector3 target = position+relativePosition;
		Vector3 interp = position.serp(target, 0.3f);

		relativePosition -= interp-position;
			
		// Update transform.
		transform->setPosition( interp );
		transform->setRotation( Quaternion(rotation) );
	}
}

//-----------------------------------//

void FirstPersonController::registerCallbacks()
{
	InputManager* input = GetInputManager();
	if( !input ) return;

	Keyboard* keyboard = input->getKeyboard();
	keyboard->onKeyPress.Connect( this, &FirstPersonController::onKeyPressed );

	Mouse* mouse = input->getMouse();
	mouse->onMouseMove.Connect( this, &FirstPersonController::onMouseMove );
	mouse->onMouseDrag.Connect( this, &FirstPersonController::onMouseDrag );
	mouse->onMouseWheelMove.Connect( this, &FirstPersonController::onMouseWheel );
}

//-----------------------------------//

void FirstPersonController::onKeyPressed( const KeyEvent& keyEvent )
{
	if( !enabled ) return;

	Window* window = GetInputManager()->getWindow();
	if( !window ) return;

	switch( keyEvent.keyCode )
	{
	case Keys::LControl:
	{
		if( window->isCursorVisible() )
		{
			oldMousePosition = window->getCursorPosition();
			window->setCursorVisible( false );
			centerCursor();
		}
		else
		{
			window->setCursorPosition( oldMousePosition );
			window->setCursorVisible( true );
		}

		break;
	}
	case Keys::N:
	{
		setMoveSensivity( moveSensivity - 5.0f );
		break;
	}
	case Keys::M:
	{
		setMoveSensivity( moveSensivity + 5.0f );
		break;
	}
	} // end switch
}

//-----------------------------------//

void FirstPersonController::onMouseWheel( const MouseWheelEvent& event )
{
	if( !enabled ) return;
	
	mouseWheel += event.delta;
}

//-----------------------------------//

void FirstPersonController::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !enabled ) return;
	
	Vector2i currentPosition( moveEvent.x, moveEvent.y );
	mouseDistance += currentPosition - lastPosition;

	centerCursor();
}

//-----------------------------------//

void FirstPersonController::onMouseDrag( const MouseDragEvent& event )
{
	if( !enabled ) return;

	MouseMoveEvent me;
	me.x = event.x;
	me.y = event.y;

	onMouseMove( me );
}

//-----------------------------------//

void FirstPersonController::centerCursor()
{
	Window* window = GetInputManager()->getWindow();
	if( !window ) return;

	lastPosition = window->getSettings().getSize() / 2;
	window->setCursorPosition( lastPosition );
}

//-----------------------------------//

NAMESPACE_ENGINE_END