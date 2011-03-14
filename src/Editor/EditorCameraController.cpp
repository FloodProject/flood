/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "EditorCameraController.h"
#include "Input/InputManager.h"
#include "Input/Mouse.h"
#include "Math/Helpers.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(EditorCameraController, CameraController)
END_CLASS()

//-----------------------------------//

EditorCameraController::EditorCameraController()
	: mouseWheel(0)
{
	Mouse* mouse = GetEngine()->getInputManager()->getMouse();
	mouse->onMouseButtonPress.Connect( this, &EditorCameraController::onMouseButtonPress );
	mouse->onMouseButtonRelease.Connect( this, &EditorCameraController::onMouseButtonRelease );
	mouse->onMouseDrag.Connect( this, &EditorCameraController::onMouseDrag );
	mouse->onMouseWheelMove.Connect( this, &EditorCameraController::onMouseWheel );
}

//-----------------------------------//

EditorCameraController::~EditorCameraController()
{
	Mouse* mouse = GetEngine()->getInputManager()->getMouse();
	mouse->onMouseButtonPress.Disconnect( this, &EditorCameraController::onMouseButtonPress );
	mouse->onMouseButtonRelease.Disconnect( this, &EditorCameraController::onMouseButtonRelease );
	mouse->onMouseDrag.Disconnect( this, &EditorCameraController::onMouseDrag );
	mouse->onMouseWheelMove.Disconnect( this, &EditorCameraController::onMouseWheel );
}

//-----------------------------------//

void EditorCameraController::_update( float delta )
{
	moveVector.zero();

	// Update movement from input devices.
	updateMouseMovement(delta);
	updateKeyboardMovement(delta);

	Vector3 orientedPosition = Matrix4x3::createRotation(accumulatedRotation) * moveVector;
	relativePosition += orientedPosition;
	
	Vector3 absolutePosition(
		fabs(relativePosition.x),
		fabs(relativePosition.y),
		fabs(relativePosition.z) );

	// Prevent errors in the interpolation.
	if( absolutePosition < 0.05f ) relativePosition.zero();

	calculateView();
}

//-----------------------------------//

void EditorCameraController::calculateView()
{
	const TransformPtr& transform = getEntity()->getTransform();
	const Vector3& currentPosition = transform->getPosition();

	Vector3 targetPosition = currentPosition + relativePosition;
	Vector3 interpPosition = targetPosition.serp(targetPosition, 0.3f);
	relativePosition -= interpPosition - currentPosition;
			
	transform->setPosition( interpPosition );
	transform->setRotation( Quaternion(accumulatedRotation) );
}

//-----------------------------------//

void EditorCameraController::updateKeyboardMovement(float delta)
{
	Window* window = GetEngine()->getRenderDevice()->getWindow();
	if( window->isCursorVisible() ) return;

	Keyboard* keyboard = GetEngine()->getInputManager()->getKeyboard();
	const std::vector<bool>& state = keyboard->getKeyState();
	
	if( state[Keys::W] ) moveVector +=  Vector3::UnitZ;
	if( state[Keys::S] ) moveVector += -Vector3::UnitZ;
	if( state[Keys::A] ) moveVector +=  Vector3::UnitX;
	if( state[Keys::D] ) moveVector += -Vector3::UnitX;
	if( state[Keys::Q] ) moveVector +=  Vector3::UnitY;
	if( state[Keys::Z] ) moveVector += -Vector3::UnitY;

	moveVector *= delta * moveSensivity;
}

//-----------------------------------//

void EditorCameraController::updateMouseMovement(float delta)
{
	// Calculate the temporary rotation from mouse look.
	Vector3 temporaryRotation( -mouseDistance.y, mouseDistance.x, 0 );
	temporaryRotation *= delta * lookSensivity;
	accumulatedRotation += temporaryRotation;

	// Restrict X-axis movement.
	float& xang = accumulatedRotation.x;
	float DEFAULT_LIMIT_XAXIS = 89.0f;
	MathClamp( xang, -DEFAULT_LIMIT_XAXIS, DEFAULT_LIMIT_XAXIS );

	moveVector += Vector3::UnitZ * mouseWheel * 100;

	mouseDistance.zero();
	mouseWheel = 0;
}

//-----------------------------------//

void EditorCameraController::onMouseButtonPress( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Right ) return;

	Window* window = GetEngine()->getRenderDevice()->getWindow();

	window->setCursorCapture(true);
	window->setCursorVisible(false);
	mousePosition = window->getCursorPosition();
}

//-----------------------------------//

void EditorCameraController::onMouseButtonRelease( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Right ) return;
	
	Window* window = GetEngine()->getRenderDevice()->getWindow();

	window->setCursorCapture(false);
	window->setCursorVisible(true);
	window->setCursorPosition(mousePosition);
}

//-----------------------------------//

void EditorCameraController::onMouseWheel( const MouseWheelEvent& event )
{
	if( !enabled ) return;
	mouseWheel += event.delta;
}

//-----------------------------------//

void EditorCameraController::onMouseDrag( const MouseDragEvent& event )
{
	if( !enabled ) return;
	if( !event.info->rightButton ) return;
	
	Vector2 deltaPosition( event.dx, event.dy );
	mouseDistance += deltaPosition;
}

//-----------------------------------//

} // end namespace