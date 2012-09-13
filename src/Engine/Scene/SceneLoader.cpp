/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Scene/SceneLoader.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(SceneLoader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

SceneLoader::SceneLoader()
{
	extensions.push_back("scene");
}

//-----------------------------------//

static Serializer* GetSerializerForStream(const Stream& stream)
{
	const String& ext = PathGetFileExtension(stream.path);

	if(ext == "json")
		return SerializerCreateJSON(AllocatorGetHeap(), 0);
	else if(ext == "bin")
		return SerializerCreateBinary(AllocatorGetHeap(), 0);

	return SerializerCreateJSON(AllocatorGetHeap(), 0);
}

//-----------------------------------//

bool SceneLoader::decode(ResourceLoadOptions& options)
{
	Serializer* serializer = GetSerializerForStream(*options.stream);
	if( !serializer ) return false;

	Scene* scene = (Scene*) options.resource;

	serializer->stream = options.stream;
	serializer->object = scene;

	Object* object = SerializerLoad(serializer);
	
	if( !object )
	{
		Deallocate(serializer);
		return false;
	}

	assert( object == scene );
	Deallocate(serializer);

	return true;
}

//-----------------------------------//

NAMESPACE_ENGINE_END

