/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Engine/Scene/Listener.h"
#include "Engine/Scene/Transform.h"
#include "Engine/Scene/Camera.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Audio/Device.h"
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
	AudioDevice* audioDevice = GetEngine()->getAudioDevice();
	audioContext = audioDevice->getMainContext();
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

	const Camera* camera = entity->getComponent<Camera>().get();
	
	if(camera)
	{
		Vector3 lookAt = camera->getLookAtVector();
		lookAt.normalize();

		audioContext->setOrientation(lookAt);
	}

	const Transform* transform = entity->getTransform().get();

	if(transform)
	{
		const Vector3& position = transform->getPosition();
		audioContext->setPosition(position);
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif