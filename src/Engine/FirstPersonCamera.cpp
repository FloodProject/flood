/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/FirstPersonCamera.h"

namespace vapor { namespace scene {

using namespace vapor::input;
using namespace vapor::math;

//-----------------------------------//

static const float DEFAULT_MOVE_SENSIVITY = 1.1f;
static const float DEFAULT_LOOK_SENSIVITY = 0.01f;

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

void FirstPersonCamera::update( float delta )
{
	Camera::update( delta );
	checkControls( delta );
}

//-----------------------------------//

void FirstPersonCamera::checkControls( double delta )
{
	TransformPtr transform = getNode()->getTransform();
	
	// Check mouse movement.
	double dt = delta * 100 * lookSensivity;

	Vector3 rotateVector( mouseDistance.y * (float)dt, mouseDistance.x * (float)dt, 0.0f );
	
	if( rotateVector != Vector3::Zero )
		transform->rotate( rotateVector );

	EulerAngles newForwardAngle( -rotateVector.x, -rotateVector.y, -rotateVector.z );

	//rotate( -rotateVectorKey );
	//EulerAngles newForwardAngle( rotateVectorKey.x, rotateVectorKey.y, rotateVectorKey.z );
	
	forwardVector *= newForwardAngle.getOrientationMatrix();
	//forwardVector.normalize();

	//if( mouseDistance.x != 0 && mouseDistance.y != 0 )
	//{
		//debug( "forangle: %f %f %f", -rotateVector.x, -rotateVector.y, -rotateVector.z );
	//if(rotateVectorKey.x != 0 || rotateVectorKey.y != 0 || rotateVectorKey.z != 0)
	//{
	//	debug( "forangle: %f %f %f", rotateVectorKey.x, rotateVectorKey.y, rotateVectorKey.z );
	//	debug( "forward: %f %f %f", forwardVector.x, forwardVector.y, forwardVector.z );
	//}
	//}

	// Restrict X-axis movement by 90 deegres.
	float xlimit = degreeToRadian( 90.0f );
	const float& xang = transform->getRotation().xang;
	limit< float >( const_cast< float& >( xang ), -xlimit, xlimit );

	mouseDistance.zero();

	// Check keyboard movement.
	Keyboard* kbd = inputManager->getKeyboard();
	const std::vector< bool >& state = kbd->getKeyState();

	/*double*/ dt = delta * 100 * moveSensivity;

	if( state[Keys::W] == true )
		transform->translate( forwardVector * (float)dt );

	if( state[Keys::S] == true )
		transform->translate( -forwardVector * (float)dt );
	
	if( state[Keys::A] == true )
		transform->translate( Vector3::UnitX * (float)dt );

	if( state[Keys::D] == true )
		transform->translate( -Vector3::UnitX * (float)dt );
	
	if( state[Keys::Q] == true )
		transform->translate( -Vector3::UnitY * (float)dt );

	if( state[Keys::Z] == true )
		transform->translate( Vector3::UnitY * (float)dt );
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
		kbd->onKeyRelease += fd::bind( &FirstPersonCamera::onKeyReleased, this );
	}
	
	if( mouse )
	{
		mouse->onMouseMove += fd::bind( &FirstPersonCamera::onMouseMove, this );
		mouse->onMouseButtonPress += fd::bind( &FirstPersonCamera::onButtonPressed, this );
		mouse->onMouseButtonRelease += fd::bind( &FirstPersonCamera::onButtonReleased, this );
		mouse->onMouseWheelMove += fd::bind( &FirstPersonCamera::onMouseWheel, this );
	}

	renderDevice->getWindow()->onWindowFocusChange += fd::bind( &FirstPersonCamera::onWindowFocusChange, this );
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
	//	case Keys::Left:
	//	{
	//		if(!check){
	//			check = true;
	//			rotateVectorKey = Vector3(0.0f, 0.005f, 0.0f);
	//			break;
	//		}
	//	}
	//	case Keys::Right:
	//	{
	//		if(!check){
	//			check = true;
	//			rotateVectorKey = Vector3(0.0f, -0.005f, 0.0f);
	//			break;
	//		}
	//	}
	//	case Keys::Up:
	//	{
	//		if(!check){
	//			check = true;
	//			rotateVectorKey = Vector3(0.005f, 0.0f, 0.0f);
	//			break;
	//		}
	//	}
	//	case Keys::Down:
	//	{
	//		if(!check){
	//			check = true;
	//			rotateVectorKey = Vector3(-0.005f, 0.0f, 0.0f);
	//			break;
	//		}
	//	}
	}
}

//-----------------------------------//

void FirstPersonCamera::onKeyReleased( const KeyEvent& keyEvent )
{
	//switch( keyEvent.keyCode )
	//{	
		//case Keys::Left:
		//case Keys::Right:
		//case Keys::Up:
		//case Keys::Down:
		//{
		//	check = false;
		//	rotateVectorKey.zero();
		//	break;
		//}
	//}
}

//-----------------------------------//

void FirstPersonCamera::onMouseWheel( const input::MouseWheelEvent& event )
{
	TransformPtr trans = getNode()->getTransform();
	trans->translate( Vector3::UnitY * (moveSensivity * event.delta) );
}

//-----------------------------------//

void FirstPersonCamera::onMouseMove( const MouseMoveEvent& moveEvent )
{
	render::Window* window = renderDevice->getWindow();

	if( !window->getCursorState() )
	{
		Vector3 currentPosition( (float)moveEvent.x, (float)moveEvent.y, 0 );
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

void FirstPersonCamera::centerCursor( )
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

void FirstPersonCamera::onWindowFocusChange( bool focusLost )
{
	if( !focusLost )
		centerCursor();
}

//-----------------------------------//

} } // end namespaces