/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

namespace vapor {

//-----------------------------------//

class ResourceTask : public Task
{
public:

	~ResourceTask()
	{

	}

	void run()
	{
		assert( rm != nullptr );

		const std::string& path = res->getPath();
		
		File file(path);
		const std::string& ext = file.getExtension();
		ResourceLoader* const loader = rm->getResourceLoader(ext);

		if( !loader )
		{
			Log::warn( "No resource loader found for resource '%s'", path.c_str() );
			return;
		}
		
		if( loader->decode(file, res) )
		{
			res->setStatus( ResourceStatus::Loaded );
		}
		else
		{
			res->setStatus( ResourceStatus::Error );
			Log::warn("Error decoding resource '%s'", path.c_str());
			return;
		}

		if( notify )
		{
			ResourceEvent event;
			event.resource = res;

			rm->resourceTaskEvents.push(event);
		}

		rm->numResourcesQueuedLoad--;
		THREAD( rm->resourceFinishLoad.notify_one(); )

		Log::info("Loaded resource '%s'", path.c_str());
	}

	Resource* res;
	ResourceManager* rm;
	bool notify;
};

//-----------------------------------//

} // end namespace