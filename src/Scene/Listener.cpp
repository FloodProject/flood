/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "Scene/Listener.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"
#include "Scene/Entity.h"
#include "Audio/Device.h"
#include "Engine.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Listener, Component)
	FIELD_PRIMITIVE_SETTER(float, volume, Volume)
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
{ }

//-----------------------------------//

void Listener::setVolume( float volume )
{
	this->volume = volume;
	audioContext->setVolume(volume);
}

//-----------------------------------//

void Listener::update( float delta )
{
	if(!audioContext)
		return;

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

} // end namespace

#endif