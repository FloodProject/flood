/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourceThumbnailer.h"
#include "Settings.h"
#include "Core/Utilities.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

ResourceThumbnailer::ResourceThumbnailer()
{
}

//-----------------------------------//

void ResourceThumbnailer::setupRender()
{
	RenderDevice* device = GetRenderDevice();

	Settings settings(ThumbSize, ThumbSize);
	renderBuffer = device->createRenderBuffer(settings);
	renderBuffer->createRenderBuffer(RenderBufferType::Depth);
	colorTexture = renderBuffer->createRenderTexture(RenderBufferType::Color);
	
	if( !renderBuffer->check() )
		return;
	
	camera.reset( new Camera() );
	Frustum& frustum = camera->getFrustum();
	frustum.nearPlane = 0.1f;

	entityCamera.reset( new Entity() );
	entityCamera->addTransform();
	entityCamera->addComponent( camera );
	
	scene.reset( new Scene() );
	scene->add( entityCamera );

	renderView = new RenderView(camera);
	renderView->setClearColor(Color::White);
	renderView->setRenderTarget(renderBuffer);
}

//-----------------------------------//
#if 0

void ResourceDatabase::generateThumbnails(const std::vector<String>& files)
{
	ResourceManager* res = GetResourceManager();

	wxProgressDialog progressDialog( "Loading resources",
		"Please wait while resources are loaded.", files.size(),
		this, wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT );
	
	progressDialog.Show();

	size_t progress = 0;

	for( size_t i = 0; i < files.size(); i++ )
	{
		const String& path = files[i];
		
		// Force unused resources to be unloaded.
		res->update(0);

		progressDialog.Update(progress++, PathUtils::getFile(path));

		if( progressDialog.WasCancelled() )
			break;

		File file(path);
		
		std::vector<byte> data;
		file.read(data);

		uint hash = Hash::Murmur2( data, 0xBEEF );
		
		if( resourcesCache.find(hash) != resourcesCache.end() )
			continue;

		ResourceLoadOptions options;
		options.name = path;
		options.asynchronousLoading = false;

		MeshPtr mesh = RefCast<Mesh>(res->loadResource(options));

		if( !mesh || mesh->getResourceGroup() != ResourceGroup::Meshes )
			continue;

		const String& resPath = PathGetFile(mesh->getPath());

		ResourceMetadata metadata;
		metadata.hash = hash;
		metadata.thumbnail = resPath + ".png";
		metadata.path = resPath;
		resourcesCache[hash] = metadata;

		ImagePtr thumb = generateThumbnail(mesh);

		if( !thumb )
			continue;

		ImageWriter writer;
		writer.save( thumb, CacheFolder + metadata.thumbnail );

		LogInfo("Generated thumbnail for resource '%s'", resPath.c_str());
	}
}
#endif

//-----------------------------------//

NAMESPACE_EDITOR_END