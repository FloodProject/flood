/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
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
static const float DEFAULT_LOOK_SENSIVITY = 0.5f;
static const float DEFAULT_LIMIT_XAXIS = degreeToRadian( 89.999f );

//-----------------------------------//

const std::string& FirstPersonCamera::type = "FirstPersonCamera";

//-----------------------------------//

FirstPersonCamera::FirstPersonCamera( input::InputManager* input,
	render::Device* device, Projection::Enum projection )
	: Camera( device, projection ), inputManager( input ), 
	moveSensivity( DEFAULT_MOVE_SENSIVITY ), lookSensivity( DEFAULT_LOOK_SENSIVITY ), 
	clampMovementX( true )
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
	// Check mouse movement.
	Vector3 rotate( mouseDistance.y, -mouseDistance.x, 0.0f );
	transform->rotate( rotate * (float(delta) * lookSensivity) );

	// Restrict X-axis movement by some deegres.
	float& xang = const_cast< float& >( transform->getRotation().xang );
	limit< float >( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

	mouseDistance.zero();

	// Check keyboard movement.
	Keyboard* kbd = inputManager->getKeyboard();
	const std::vector< bool >& state = kbd->getKeyState();

	Vector3 moveVector;
	if( state[Keys::W] )
		moveVector+=( Vector3::UnitZ );
	
	if( state[Keys::S] )
		moveVector+=( -Vector3::UnitZ );
	
	if( state[Keys::A] )
		moveVector+=( Vector3::UnitX );

	if( state[Keys::D] )
		moveVector+=( -Vector3::UnitX );
	
	if( state[Keys::Q] )
		moveVector+=( Vector3::UnitY );

	if( state[Keys::Z] )
		moveVector+=( -Vector3::UnitY );

/*	if( state[Keys::Up] )
		transform->rotate( -0.01f, 0.0f, 0.0f );

	if( state[Keys::Down] )
		transform->rotate( 0.01f, 0.0f, 0.0f );
    
	if( state[Keys::Left] )
		transform->rotate( 0.0f, 0.01f, 0.0f );

	if( state[Keys::Right] )
		transform->rotate( 0.0f, -0.01f, 0.0f );*/	

	// Handle the queued movement from mouse wheel
	moveVector += queuedMovement;
	queuedMovement.zero();

	moveVector *= (float(delta) * moveSensivity);
	EulerAngles& rotAng =  const_cast< EulerAngles& >(transform->getRotation());
	transform->translate( moveVector*rotAng.getOrientationMatrix() );
	
	Vector3 transformedReference = Vector3::UnitZ * rotAng.getOrientationMatrix();
	Vector3 cameraLookAt = transform->getPosition() + transformedReference;
	viewMatrix = transform->lookAt( cameraLookAt, Vector3::UnitY );
}

//-----------------------------------//

void FirstPersonCamera::registerCallbacks()
{
	Keyboard* kbd = inputManager->getKeyboard();
	Mouse* mouse = inputManager->getMouse();

	assert( kbd != nullptr );
	assert( mouse != nullptr );
	
	kbd->onKeyPress += fd::bind( &FirstPersonCamera::onKeyPressed, this );
	mouse->onMouseMove += fd::bind( &FirstPersonCamera::onMouseMove, this );
	mouse->onMouseWheelMove += fd::bind( &FirstPersonCamera::onMouseWheel, this );

	render::Window* window = renderDevice->getWindow();
	window->onWindowFocusChange += fd::bind( &FirstPersonCamera::onWindowFocusChange, this );
}

//-----------------------------------//

void FirstPersonCamera::onKeyPressed( const KeyEvent& keyEvent )
{
	switch( keyEvent.keyCode )
	{	
		case Keys::LControl:
		{
			render::Window* window = renderDevice->getWindow();
			window->setCursorVisible( !window->isCursorVisible() );
			centerCursor();
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
	}
}

//-----------------------------------//

void FirstPersonCamera::onMouseWheel( const input::MouseWheelEvent& event )
{
	if( !transform ) return;
	queuedMovement += Vector3::UnitZ * event.delta * 100;
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	render::Window* window = renderDevice->getWindow();

	if( window->isCursorVisible() ) return;
		
	Vector3 currentPosition( moveEvent.x, moveEvent.y, 0 );
	mouseDistance += currentPosition - lastPosition;
	lastPosition = currentPosition;

	centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::centerCursor( )
{
	render::Window* window = renderDevice->getWindow();

	if( window->isCursorVisible() ) return;

	int x = window->getSettings().getWidth() / 2;
	int y = window->getSettings().getHeight() / 2;
	
	lastPosition.x = x;
	lastPosition.y = y;
	
	window->setCursorPosition( x, y );
}

//-----------------------------------//

void FirstPersonCamera::onWindowFocusChange( bool focusLost )
{
	if( !focusLost )
		centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::setLookSensivity( float sensivity )
{
	lookSensivity = sensivity;
}

//-----------------------------------//

void FirstPersonCamera::setMoveSensivity( float sensivity )
{
	moveSensivity = sensivity;
}

//-----------------------------------//

const std::string& FirstPersonCamera::getType() const
{
	return FirstPersonCamera::type;
}

//-----------------------------------//

const std::string FirstPersonCamera::save(int ind)
{
	return "";
}

//-----------------------------------//

} } // end namespaces