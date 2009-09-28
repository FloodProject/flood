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

namespace vapor {
	namespace scene {

//-----------------------------------//

FirstPersonCamera::FirstPersonCamera( input::InputManager* input,
	render::Device* device, 
	Projection::Enum projection )
	: Camera( device, projection ), inputManager( input ), lastMoveEvent( 0, 0 )
{
	registerCallbacks();
}

//-----------------------------------//

void FirstPersonCamera::update( double delta )
{
	Camera::update( delta );
}

//-----------------------------------//

const std::string FirstPersonCamera::name() const
{
	return "FPCamera";
}

//-----------------------------------//

const std::string FirstPersonCamera::save(int ind)
{
	return "";
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
	if( keyEvent.keyCode == Keys::W )
		translate( math::Vector3( 0.0f, 0.0f, 1.0f ) );

	if( keyEvent.keyCode == Keys::S )
		translate( math::Vector3( 0.0f, 0.0f, -1.0f ) );

	if( keyEvent.keyCode == Keys::A )
		translate( math::Vector3( 1.0f, 0.0f, 0.0f ) );

	if( keyEvent.keyCode == Keys::D )
		translate( math::Vector3( -1.0f, 0.0f, 0.0f ) );

	if( keyEvent.keyCode == Keys::Q )
		translate( math::Vector3( 0.0f, -1.0f, 0.0f ) );

	if( keyEvent.keyCode == Keys::Z )
		translate( math::Vector3( 0.0f, 1.0f, 0.0f ) );

	if( keyEvent.keyCode == Keys::LControl )
	{
		render::Window* window = renderDevice->getWindow();
		window->setCursorState( !window->getCursorState() );
	}
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	//moveEvent.x
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

} } // end namespaces