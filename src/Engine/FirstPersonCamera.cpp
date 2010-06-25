/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/FirstPersonCamera.h"
#include "vapor/scene/Node.h"
#include "vapor/math/Math.h"

namespace vapor {

//-----------------------------------//

static const float DEFAULT_MOVE_SENSIVITY = 100.0f;
static const float DEFAULT_LOOK_SENSIVITY = 20.0f;
static const float DEFAULT_LIMIT_XAXIS = 89.0f;

const std::string& FirstPersonCamera::type = "FirstPersonCamera";

//-----------------------------------//

FirstPersonCamera::FirstPersonCamera( RenderDevice* device,
									  Projection::Enum projection )
	: Camera( device, projection ),
	clampMovementX( true ),
	moveSensivity( DEFAULT_MOVE_SENSIVITY ), 
	lookSensivity( DEFAULT_LOOK_SENSIVITY ),
	mouseWheel(0)
{
	Window* window = device->getWindow();
	inputManager = window->getInputManager();

	registerCallbacks();
}

//-----------------------------------//

void FirstPersonCamera::update( double delta )
{
	Camera::update( delta );

	if( hasFocus )
		checkControls( delta );
}

//-----------------------------------//

void FirstPersonCamera::checkControls( double delta )
{
	bool viewChanged = false;
	Vector3 moveVector;
	
	Vector3 position = transform->getPosition();
	EulerAngles rotation = transform->getRotation();

	// Check mouse movement.
	if( mouseDistance != Vector2i::Zero )
	{
		Vector3 rotate( float(mouseDistance.y), float(-mouseDistance.x), 0.0f );
		rotation += rotate * (float(delta) * lookSensivity);

		// Restrict X-axis movement by some deegres.
		// TODO: this screws the movement when we are looking down
		float& xang = rotation.x;
		clamp<float>( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

		mouseDistance.zero();
		viewChanged = true;
	}

	// Check mouse wheel movement.
	if( mouseWheel != 0 )
	{
		moveVector += Vector3::UnitZ * float(mouseWheel) * 100;
		mouseWheel = 0;
	}

	// Check keyboard movement.
	Keyboard* const kbd = inputManager->getKeyboard();
	const std::vector< bool >& state = kbd->getKeyState();

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
		moveVector *= float(delta) * moveSensivity;
		position += moveVector * rotation.getOrientationMatrix();
		viewChanged = true;
	}

	if( viewChanged )
	{
		// Update transform.
		transform->setPosition( position );
		transform->setRotation( rotation );
	}
}

//-----------------------------------//

void FirstPersonCamera::registerCallbacks()
{
	Keyboard* const keyboard = inputManager->getKeyboard();

	if( keyboard )
	{
		keyboard->onKeyPress +=
			fd::bind( &FirstPersonCamera::onKeyPressed, this );
	}

	Mouse* const mouse = inputManager->getMouse();

	if( mouse )
	{
		mouse->onMouseMove +=
			fd::bind( &FirstPersonCamera::onMouseMove, this );
		
		mouse->onMouseDrag +=
			fd::bind( &FirstPersonCamera::onMouseDrag, this );
		
		mouse->onMouseWheelMove +=
			fd::bind( &FirstPersonCamera::onMouseWheel, this );
	}

	Window* window = renderDevice->getWindow();
	assert( window != nullptr );

	window->onWindowFocusChange +=
		fd::bind( &FirstPersonCamera::onWindowFocusChange, this );
}

//-----------------------------------//

void FirstPersonCamera::onKeyPressed( const KeyEvent& keyEvent )
{
	switch( keyEvent.keyCode )
	{

	case Keys::LControl:
	{
		Window* window = renderDevice->getWindow();
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

void FirstPersonCamera::onMouseWheel( const MouseWheelEvent& event )
{
	mouseWheel += event.delta;
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	Window* window = renderDevice->getWindow();
	
	if( window->isCursorVisible() )
		return;
		
	Vector2i currentPosition( moveEvent.x, moveEvent.y );
	mouseDistance += currentPosition - lastPosition;

	centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::onMouseDrag( const MouseDragEvent& event )
{
	MouseMoveEvent me;
	me.x = event.x;
	me.y = event.y;

	onMouseMove( me );
}

//-----------------------------------//

void FirstPersonCamera::centerCursor( )
{
	Window* window = renderDevice->getWindow();
	lastPosition = window->getSettings().getSize() / 2;
	window->setCursorPosition( lastPosition );
}

//-----------------------------------//

void FirstPersonCamera::onWindowFocusChange( bool focusLost )
{
	hasFocus = !focusLost;

	Window* window = renderDevice->getWindow();
	
	if( hasFocus && !window->isCursorVisible() )
		centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::serialize( Json::Value& value )
{
	Camera::serialize( value );

	value["lookSensivity"] = lookSensivity;
	value["moveSensivity"] = moveSensivity;
}

//-----------------------------------//

} // end namespace