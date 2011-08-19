/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Scene/Listener.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Scene/Entity.h"
#include "Audio/Device.h"
#include "Engine/Engine.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Listener, Component)
	FIELD_PRIMITIVE_SETTER(4, float, volume, Volume)
REFLECT_CLASS_END()

//-----------------------------------//

Listener::Listener()
	: volume(0.75f)
	, audioContext(nullptr)
{
#if 0
	AudioDevice* audioDevice = GetEngine()->getAudioDevice();
	audioContext = audioDevice->getMainContext();
#endif
}

//-----------------------------------//

Listener::~Listener()
{ 

}

//-----------------------------------//

void Listener::setVolume( float volume )
{
	this->volume = volume;

	if( !audioContext ) return;
	audioContext->setVolume(volume);
}

//-----------------------------------//

void Listener::update( float delta )
{
	if(!audioContext) return;

	#pragma TODO(Get orientation from the transform)

	const CameraPtr& camera = entity->getComponent<Camera>();
	
	if(camera)
	{
		Vector3 lookAt = camera->getLookAtVector();
		lookAt.normalize();

		audioContext->setOrientation(lookAt);
	}

	const TransformPtr& transform = entity->getTransform();

	if(transform)
	{
		const Vector3& position = transform->getPosition();
		audioContext->setPosition(position);
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif