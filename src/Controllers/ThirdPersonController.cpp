/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Controllers/ThirdPersonController.h"

#include "Engine/Engine.h"
#include "Math/Helpers.h"
#include "Scene/Entity.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "Render/Device.h"
#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ThirdPersonController, CameraController)
	FIELD_PRIMITIVE(Vector3, distance)
	FIELD_PRIMITIVE(Vector3, rotation)
	FIELD_PRIMITIVE(string, followEntityName)
REFLECT_CLASS_END()

//-----------------------------------//

ThirdPersonController::ThirdPersonController()
{ }

//-----------------------------------//

void ThirdPersonController::_update( float delta )
{
#if 0
	if( !followEntity.lock() )
	{
		Engine* engine = GetEngine();
		const ScenePtr& scene = engine->getSceneManager();
		followEntity = scene->findEntity(followEntityName);
	}

	const EntityPtr& node = followEntity.lock();

	if( !node )
		return;

	const TransformPtr& followTransform = node->getTransform();
	const TransformPtr& transform = getEntity()->getTransform();

	transform->setPosition( followTransform->getPosition() + distance );
	transform->setRotation( followTransform->getRotation() * Quaternion(rotation) );
#endif
//
//	Vector3 thirdPersonReference = new Vector3(0, 200, -200);
//	Matrix rotationMatrix = Matrix.CreateRotationY(avatarYaw);
//
//	// Create a vector pointing the direction the camera is facing.
//	Vector3 transformedReference = Vector3.Transform(thirdPersonReference, rotationMatrix);
//
//	// Calculate the position the camera is looking from.
//	Vector3 cameraPosition = transformedReference + avatarPosition;
//
//	view = Matrix.CreateLookAt(cameraPosition, avatarPosition, 
//	    new Vector3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------//

NAMESPACE_ENGINE_END