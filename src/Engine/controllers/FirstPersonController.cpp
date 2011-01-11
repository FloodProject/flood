/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "Event.h"
#include "vapor/controllers/FirstPersonController.h"
#include "vapor/Engine.h"
#include "vapor/math/Math.h"
#include "vapor/scene/Entity.h"
#include "vapor/scene/Camera.h"
#include "vapor/render/Device.h"
#include "vapor/input/InputManager.h"
#include "vapor/input/Keyboard.h"
#include "vapor/input/Mouse.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(FirstPersonController, CameraController)
END_CLASS()

//-----------------------------------//

static const float DEFAULT_LIMIT_XAXIS = 89.0f;

//-----------------------------------//

FirstPersonController::FirstPersonController()
	: clampMovementX( true )
	, mouseWheel(0)
{
	Engine* engine = Engine::getInstancePtr();

	window = engine->getRenderDevice()->getWindow();
	inputManager = engine->getInputManager();

	registerCallbacks();
}

//-----------------------------------//

FirstPersonController::~FirstPersonController()
{
	window->onWindowFocusChange.Disconnect( this, &FirstPersonController::onWindowFocusChange );

	if(!inputManager)
		return;

	Keyboard* const keyboard = inputManager->getKeyboard();
	assert( keyboard != nullptr );

	Mouse* const mouse = inputManager->getMouse();
	assert( mouse != nullptr );
	
	keyboard->onKeyPress.Disconnect( this, &FirstPersonController::onKeyPressed );
	mouse->onMouseMove.Disconnect( this, &FirstPersonController::onMouseMove );
	mouse->onMouseDrag.Disconnect( this, &FirstPersonController::onMouseDrag );
	mouse->onMouseWheelMove.Disconnect( this, &FirstPersonController::onMouseWheel );
}

//-----------------------------------//

void FirstPersonController::_update( double delta )
{
	if( hasFocus )
		checkControls( delta );
}

//-----------------------------------//

void FirstPersonController::checkControls( double delta )
{
	if(!inputManager)
		return;

	const TransformPtr& transform = getEntity()->getTransform();
	Vector3 position = transform->getPosition();
	
	Vector3 moveVector;
	bool viewChanged = false;
	
	// Check mouse movement.
	if( mouseDistance != Vector2i::Zero )
	{
		Vector3 rotate( mouseDistance.y, -mouseDistance.x, 0 );
		rotate *= delta * lookSensivity;

		rotation += rotate;

		// Restrict X-axis movement by some deegres.
		float& xang = rotation.x;
		Math::clamp( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

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
	Keyboard* keyboard = inputManager->getKeyboard();
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
	if(!window || !inputManager)
		return;

	window->onWindowFocusChange.Connect( this, &FirstPersonController::onWindowFocusChange );

	Keyboard* const keyboard = inputManager->getKeyboard();
	assert( keyboard != nullptr );

	Mouse* const mouse = inputManager->getMouse();
	assert( mouse != nullptr );
	
	keyboard->onKeyPress.Connect( this, &FirstPersonController::onKeyPressed );
	mouse->onMouseMove.Connect( this, &FirstPersonController::onMouseMove );
	mouse->onMouseDrag.Connect( this, &FirstPersonController::onMouseDrag );
	mouse->onMouseWheelMove.Connect( this, &FirstPersonController::onMouseWheel );
}

//-----------------------------------//

void FirstPersonController::onKeyPressed( const KeyEvent& keyEvent )
{
	if( !enabled )
		return;

	switch( keyEvent.keyCode )
	{

	case Keys::LControl:
	{
		assert( window != nullptr );

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
	if( !enabled )
		return;

	mouseWheel += event.delta;
}

//-----------------------------------//

void FirstPersonController::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !enabled )
		return;

	if( window->isCursorVisible() )
		return;
		
	Vector2i currentPosition( moveEvent.x, moveEvent.y );
	mouseDistance += currentPosition - lastPosition;

	centerCursor();
}

//-----------------------------------//

void FirstPersonController::onMouseDrag( const MouseDragEvent& event )
{
	if( !enabled )
		return;

	MouseMoveEvent me;
	me.x = event.x;
	me.y = event.y;

	onMouseMove( me );
}

//-----------------------------------//

void FirstPersonController::centerCursor( )
{
	lastPosition = window->getSettings().getSize() / 2;
	window->setCursorPosition( lastPosition );
}

//-----------------------------------//

void FirstPersonController::onWindowFocusChange( bool focusLost )
{
	if( !enabled )
		return;
	
	hasFocus = !focusLost;

	if( hasFocus && !window->isCursorVisible() )
		centerCursor();
}

//-----------------------------------//

} // end namespace