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

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

void ResourceTaskRun(Task* task)
{
	ResourceLoadOptions* options = (ResourceLoadOptions*) task->userdata;

	const ResourceHandle& handle = options->handle;
	Stream* stream = options->stream;

	Resource* resource = handle.Resolve();
	const Path& path = resource->getPath();
	
	ResourceManager* res = GetResourceManager();
	ResourceLoader* loader = res->findLoader( PathGetFileExtension(path) );

	bool decoded = loader->decode(*stream, resource);
		
	if( !decoded )
	{
		resource->setStatus( ResourceStatus::Error );
		LogWarn("Error decoding resource '%s'", path.c_str());
		goto cleanup;
	}

	resource->setStatus( ResourceStatus::Loaded );

	if( options->sendLoadEvent )
	{
		ResourceEvent event(handle);
		res->resourceTaskEvents.push(event);
	}

	LogInfo("Loaded resource '%s'", path.c_str());

cleanup:

	AtomicDecrement(&res->numResourcesQueuedLoad);
	ConditionWakeOne(res->resourceFinishLoad);

	Deallocate(options);
	StreamDestroy(stream);
}

//-----------------------------------//

NAMESPACE_RESOURCES_END