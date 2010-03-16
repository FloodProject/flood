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

namespace vapor { namespace scene {

using namespace vapor::input;
using namespace vapor::math;

//-----------------------------------//

static const float DEFAULT_MOVE_SENSIVITY = 100.0f;
static const float DEFAULT_LOOK_SENSIVITY = 20.0f;
static const float DEFAULT_LIMIT_XAXIS = 89.0f;

const std::string& FirstPersonCamera::type = "FirstPersonCamera";

//-----------------------------------//

FirstPersonCamera::FirstPersonCamera( input::InputManager* input,
	render::Device* device, Projection::Enum projection )
	: Camera( device, projection ), inputManager( input ),
	clampMovementX( true ), moveSensivity( DEFAULT_MOVE_SENSIVITY ), 
	lookSensivity( DEFAULT_LOOK_SENSIVITY )
{
	registerCallbacks();
	centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::update( double delta )
{
	Camera::update( delta );
	checkControls( delta );
}

//-----------------------------------//

void FirstPersonCamera::checkControls( double delta )
{
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
		limit<float>( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

		mouseDistance.zero();
		viewChanged = true;
	}

	// Check mouse wheel movement.
	if( wheelMovement != Vector2i::Zero )
	{
		moveVector += wheelMovement;
		wheelMovement.zero();
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

	// If the view changed, update view matrix.
	if( viewChanged )
	{
		forwardVector = Vector3::UnitZ * rotation.getOrientationMatrix();
		lookAtVector = position + forwardVector;

		// Update transform.
		transform->setPosition( position );
		transform->setRotation( rotation );
		viewMatrix = transform->lookAt( lookAtVector, Vector3::UnitY );
		
		viewChanged = false;
	}
}

//-----------------------------------//

void FirstPersonCamera::registerCallbacks()
{
	Keyboard* const kbd = inputManager->getKeyboard();
	Mouse* const mouse = inputManager->getMouse();

	assert( kbd != nullptr );
	assert( mouse != nullptr );
	
	kbd->onKeyPress += fd::bind( &FirstPersonCamera::onKeyPressed, this );
	mouse->onMouseMove += fd::bind( &FirstPersonCamera::onMouseMove, this );
	mouse->onMouseDrag += fd::bind( &FirstPersonCamera::onMouseDrag, this );
	mouse->onMouseWheelMove += fd::bind( &FirstPersonCamera::onMouseWheel, this );

	render::Window& window = renderDevice->getWindow();
	window.onWindowFocusChange += fd::bind( &FirstPersonCamera::onWindowFocusChange, this );
}

//-----------------------------------//

void FirstPersonCamera::onKeyPressed( const KeyEvent& keyEvent )
{
	switch( keyEvent.keyCode )
	{

	case Keys::LControl:
	{
		render::Window& window = renderDevice->getWindow();

		if( window.isCursorVisible() )
		{
			oldMousePosition = window.getCursorPosition();
			lastPosition = oldMousePosition;
			centerCursor();
			window.setCursorVisible( false );
		}
		else
		{
			window.setCursorPosition( oldMousePosition.x, oldMousePosition.y );
			window.setCursorVisible( true );
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

void FirstPersonCamera::onMouseWheel( const input::MouseWheelEvent& event )
{
	if( !transform ) return;
	wheelMovement += Vector3::UnitZ * event.delta * 100;
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	const render::Window& window = renderDevice->getWindow();

	if( window.isCursorVisible() ) return;
		
	Vector2i currentPosition( moveEvent.x, moveEvent.y );
	mouseDistance += currentPosition - lastPosition;
	lastPosition = currentPosition;

	centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::onMouseDrag( const MouseDragEvent& event )
{
	MouseMoveEvent me( event.x, event.y );
	onMouseMove( me );
}

//-----------------------------------//

void FirstPersonCamera::centerCursor( )
{
	render::Window& window = renderDevice->getWindow();
	if( window.isCursorVisible() ) return;
	lastPosition = window.getSettings().getSize();
	window.setCursorPosition( lastPosition.x, lastPosition.y );
}

//-----------------------------------//

void FirstPersonCamera::onWindowFocusChange( bool focusLost )
{
	if( !focusLost )
		centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::serialize( Json::Value& value )
{
	Camera::serialize( value );

	value["lookSensivity"] = lookSensivity;
	value["moveSensivity"] = moveSensivity;
	value["lookAt"] = toJson(lookAtVector);
	value["forward"] = toJson(forwardVector);
}

//-----------------------------------//

const std::string& FirstPersonCamera::getType() const
{
	return FirstPersonCamera::type;
}

//-----------------------------------//

} } // end namespaces