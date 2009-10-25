/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/FirstPersonCamera.h"

using namespace vapor::input;
using namespace vapor::math;

namespace vapor {
	namespace scene {

//-----------------------------------//

FirstPersonCamera::FirstPersonCamera( input::InputManager* input,
	render::Device* device, Projection::Enum projection )
	: Camera( device, projection ), inputManager( input ), 
	moveSensivity( 1.0f ), lookSensivity( 0.01f ), clampMovementX( true )
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
	double dt = delta * 100 * lookSensivity;

	Vector3 rotateVector( mouseDistance.y * dt, mouseDistance.x * dt, 0.0f );
	rotate( rotateVector );

	EulerAngles newForwardAngle( -rotateVector.x, -rotateVector.y, -rotateVector.z );

	forwardVector *= newForwardAngle.getOrientationMatrix();
	//forwardVector.normalize();

	if( mouseDistance.x != 0 && mouseDistance.y != 0 )
	{
		debug( "forangle: %f %f %f", -rotateVector.x, -rotateVector.y, -rotateVector.z );
		debug( "forward: %f %f %f", forwardVector.x, forwardVector.y, forwardVector.z );
	}

	// Restrict X-axis movement by 90 deegres.
	float xlimit = degreeToRadian( 90.0f );
	limit< float >( angles.xang, -xlimit, xlimit );

	mouseDistance.zero();

	// Check keyboard movement.
	Keyboard* kbd = inputManager->getKeyboard();
	const std::vector< bool >& state = kbd->getKeyState();



	/*double*/ dt = delta * 100 * moveSensivity;

	if( state[Keys::W] == true )
		translate( forwardVector * dt );

	if( state[Keys::S] == true )
		translate( -forwardVector * dt );
	
	if( state[Keys::A] == true )
		translate( Vector3::UnitX * dt );

	if( state[Keys::D] == true )
		translate( -Vector3::UnitX * dt );
	
	if( state[Keys::Q] == true )
		translate( -Vector3::UnitY * dt );

	if( state[Keys::Z] == true )
		translate( Vector3::UnitY * dt );
}

//-----------------------------------//

const std::string FirstPersonCamera::name() const
{
	return "FPCamera";
}

//-----------------------------------//

const std::string FirstPersonCamera::save(int ind)
{
	return "camera";
}

//-----------------------------------//

void FirstPersonCamera::setLookSensivity( float sensivity )
{
	this->lookSensivity = sensivity;
}

//-----------------------------------//

void FirstPersonCamera::setMoveSensivity( float sensivity )
{
	this->moveSensivity = sensivity;
}

//-----------------------------------//

void FirstPersonCamera::registerCallbacks()
{
	Keyboard* kbd = inputManager->getKeyboard();
	Mouse* mouse = inputManager->getMouse();

	if( kbd )
	{
		kbd->onKeyPress += fd::bind( &FirstPersonCamera::onKeyPressed, this );
	}
	
	if( mouse )
	{
		mouse->onMouseMove += fd::bind( &FirstPersonCamera::onMouseMove, this );
		mouse->onMouseButtonPress += fd::bind( &FirstPersonCamera::onButtonPressed, this );
		mouse->onMouseButtonRelease += fd::bind( &FirstPersonCamera::onButtonReleased, this );
	}
}

//-----------------------------------//

void FirstPersonCamera::onKeyPressed( const KeyEvent& keyEvent )
{
	switch( keyEvent.keyCode )
	{	
		case Keys::LControl:
		{
			render::Window* window = renderDevice->getWindow();
			window->setCursorState( !window->getCursorState() );
			centerCursor();
			break;
		}
		case Keys::N:
		{
			setLookSensivity( lookSensivity - 0.5f );
			break;
		}
		case Keys::M:
		{
			setLookSensivity( lookSensivity + 0.5f );
			break;
		}
	}
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	render::Window* window = renderDevice->getWindow();

	if( !window->getCursorState() )
	{
		Vector3 currentPosition( moveEvent.x, moveEvent.y, 0 );
		mouseDistance += currentPosition - lastPosition;
		lastPosition = currentPosition;
	}

	centerCursor();
}

//-----------------------------------//

void FirstPersonCamera::onButtonPressed( const MouseButtonEvent& buttonEvent )
{

}

//-----------------------------------//

void FirstPersonCamera::onButtonReleased( const MouseButtonEvent& buttonEvent )
{

}

//-----------------------------------//

void FirstPersonCamera::centerCursor()
{
	render::Window* window = renderDevice->getWindow();

	if( !window->getCursorState() )
	{
		int nw = window->getSettings().getWidth() / 2;
		int nh = window->getSettings().getHeight() / 2;
		
		window->setCursorPosition( nw, nh );

		lastPosition.x = static_cast<float>( nw );
		lastPosition.y = static_cast<float>( nh );
	}
}

//-----------------------------------//

} } // end namespaces