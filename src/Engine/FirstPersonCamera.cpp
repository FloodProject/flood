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

namespace vapor { namespace scene {

using namespace vapor::input;
using namespace vapor::math;

//-----------------------------------//

static const float DEFAULT_MOVE_SENSIVITY = 0.5f;
static const float DEFAULT_LOOK_SENSIVITY = 0.005f;
static const float DEFAULT_LIMIT_XAXIS = degreeToRadian( 89.0f );

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
	float dt = delta * 100.0f * lookSensivity;

	Vector3 rotateVector( mouseDistance.y * dt, -mouseDistance.x * dt, 0.0f );
	transform->rotate( rotateVector );

	//// Restrict X-axis movement by some deegres.
	float& xang = const_cast< float& >( transform->getRotation().xang );
	limit< float >( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

	mouseDistance.zero();

	// Check keyboard movement.
	Keyboard* kbd = inputManager->getKeyboard();
	const std::vector< bool >& state = kbd->getKeyState();

	dt = delta * 100.0f * moveSensivity;

	Vector3 moveVector;

	if( state[Keys::W] == true )
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

	if( state[Keys::I] )
		transform->rotate( -0.01f, 0.0f, 0.0f );

	if( state[Keys::K] )
		transform->rotate( 0.01f, 0.0f, 0.0f );
    
	if( state[Keys::J] )
		transform->rotate( 0.0f, 0.01f, 0.0f );

	if( state[Keys::L] )
		transform->rotate( 0.0f, -0.01f, 0.0f );	

	Matrix4x3 t = EulerAngles(30.0f,20.0f,0.0f).getOrientationMatrix();
	Vector3 fuck = Vector3(10.0f, 20.0f, 30.0f)*t;

	moveVector = moveVector * dt;
	EulerAngles& rotAng =  const_cast< EulerAngles& >(transform->getRotation());
	transform->translate( moveVector*rotAng.getOrientationMatrix() );
	
	{
		Vector3 transformedReference = Vector3::UnitZ * rotAng.getOrientationMatrix();
		Vector3 cameraLookAt = transform->getPosition() + transformedReference;
		viewMatrix = transform->lookAt( cameraLookAt, Vector3::UnitY );


		debug( "ang: %f, %f, %f", rotAng.xang, rotAng.yang, rotAng.zang );
		debug( "lookAt: %f, %f, %f", transformedReference.x, transformedReference.y, transformedReference.z );
	}

	rotAng.canonize();

	//viewMatrix = transform->lookAt( Vector3::UnitZ, Vector3::UnitY );
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
	switch( keyEvent.keyCode )
	{	
		case Keys::Left:
		case Keys::Right:
		case Keys::Up:
		case Keys::Down:
		{
			//check = false;
			//rotateVectorKey.zero();
			//break;
		}
	}
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
		Vector3 currentPosition( moveEvent.x, moveEvent.y, 0 );
		mouseDistance += currentPosition - lastPosition;
		lastPosition = currentPosition;
	}

	centerCursor();
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