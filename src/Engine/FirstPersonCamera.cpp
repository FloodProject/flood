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
	sensivity( 5.0f ), cameraSensivity( 0.002f )
{
	registerCallbacks();
	
	//const input::MouseInfo& info = input->getMouse()->getMouseInfo();
	//lastPosition.x = info.x;
	//lastPosition.y = info.y;
}

//-----------------------------------//

void FirstPersonCamera::update( double delta )
{
	Camera::update( delta );
	centerCursor();
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

void FirstPersonCamera::setSensivity( float sensivity )
{
	this->sensivity = sensivity;
}

//-----------------------------------//

void FirstPersonCamera::registerCallbacks()
{
	Keyboard* kbd = inputManager->getKeyboard();
	Mouse* mouse = inputManager->getMouse();

	if( kbd )
	{
		kbd->onKeyPress.bind( &FirstPersonCamera::onKeyPressed, this );
	}
	
	if( mouse )
	{
		mouse->onMouseMove.bind( &FirstPersonCamera::onMouseMove, this );
		mouse->onMouseButtonPress.bind( &FirstPersonCamera::onButtonPressed, this );
		mouse->onMouseButtonRelease.bind( &FirstPersonCamera::onButtonReleased, this );
	}
}

//-----------------------------------//

void FirstPersonCamera::onKeyPressed( const KeyEvent& keyEvent )
{
	switch( keyEvent.keyCode )
	{
	
	case Keys::W:
		translate( math::Vector3( 0.0f, 0.0f, 1.0f * sensivity ) );
		break;
	case Keys::S:
		translate( math::Vector3( 0.0f, 0.0f, -1.0f * sensivity ) );
		break;
	case Keys::A:
		translate( math::Vector3( 1.0f * sensivity, 0.0f, 0.0f ) );
		break;
	case Keys::D:
		translate( math::Vector3( -1.0f * sensivity, 0.0f, 0.0f ) );
		break;
	
	case Keys::Q:
		translate( math::Vector3( 0.0f, -1.0f * sensivity, 0.0f ) );
		break;
	case Keys::Z:
		translate( math::Vector3( 0.0f, 1.0f * sensivity, 0.0f ) );
		break;
	
	case Keys::LControl:
	{
		render::Window* window = renderDevice->getWindow();
		window->setCursorState( !window->getCursorState() );
		centerCursor();
		break;
	}
	
	case Keys::N:
		setSensivity( sensivity - 0.5f );
		break;
	case Keys::M:
		setSensivity( sensivity + 0.5f );
		break;
	}
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	Vector3 currentPosition( moveEvent.x, moveEvent.y, 0 );
	
	Vector3 delta = currentPosition - lastPosition;
	lastPosition = currentPosition;

	rotate( delta.y * cameraSensivity, delta.x * cameraSensivity, 0 );
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