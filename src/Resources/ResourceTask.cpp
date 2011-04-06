/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"

namespace vapor {

//-----------------------------------//

void ResourceTaskRun(Task* task)
{
	ResourceLoadOptions* options = (ResourceLoadOptions*) task->Userdata;
	Resource* resource = options->resource;
	const Path& path = resource->getPath();
	
#ifdef VAPOR_VFS_PHYSFS
	Stream* stream = StreamCreateFromPhysfs( AllocatorGetHeap(), options->name, StreamMode::Read);
#else
	Stream* stream = StreamCreateFromFile( AllocatorGetHeap(), options->name, StreamMode::Read);
#endif

	ResourceManager* res = GetResourceManager();
	ResourceLoader* loader = res->findLoader( PathGetFileExtension(path) );

	bool decoded = loader->decode(*stream, resource);
		
	if( !decoded )
	{
		resource->setStatus( ResourceStatus::Error );
		LogWarn("Error decoding resource '%s'", path.c_str());
		return;
	}

	resource->setStatus( ResourceStatus::Loaded );

	if( options->sendLoadEvent )
	{
		ResourceEvent event;
		event.resource = resource;

		res->resourceTaskEvents.push(event);
	}

	AtomicDecrement(&res->numResourcesQueuedLoad);
	ConditionWakeOne(&res->resourceFinishLoad);

	LogInfo("Loaded resource '%s'", path.c_str());

	Deallocate( AllocatorGetHeap(), options );
	StreamDestroy(stream, AllocatorGetHeap());
}

//-----------------------------------//

} // end namespace