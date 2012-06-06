/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourceThumbnailer.h"
#include "Resources/ResourceIndexer.h"
#include "Core/Utilities.h"
#include "Graphics/RenderBuffer.h"
#include "EditorSettings.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

ResourceThumbnailer::ResourceThumbnailer()
	: indexer(nullptr)
{
}

//-----------------------------------//

void ResourceThumbnailer::setIndexer(ResourceIndexer* indexer)
{
	assert(indexer != nullptr);

	this->indexer = indexer;
	indexer->onResourceIndexed.Connect(this, &ResourceThumbnailer::onResourceIndexed);
}

//-----------------------------------//

void ResourceThumbnailer::update()
{
	//generateThumbnail();
}

//-----------------------------------//

void ResourceThumbnailer::onResourceIndexed(const ResourceMetadata& res)
{
	resourcesIndexed.push_back(res);
}

//-----------------------------------//

void ResourceThumbnailer::generateThumbnail()
{
	ResourceMetadata res;
	
	if( !resourcesIndexed.try_pop_front(res) )
		return;

	// For each resource indexed, we check if the thumbnail needs to be rebuilt.
	// The thumbnails are named with the hash of the resource, so to check just
	// search for a file with  the same name as the hash of the resource.
	
	if( !res.preview.empty() ) return;

	String hashName = StringFromNumber(res.hash) + ".png";
	Path hashPath = PathCombine(CacheFolder, hashName);

	if( FileExists(hashPath) ) return;

	res.preview = hashPath;

	ResourceManager* rm = GetResourceManager();
	rm->setAsynchronousLoading(false);

	ResourceLoadOptions options;
	options.name = PathGetFile(res.path);
	//options.asynchronousLoad = false;

	ImagePtr thumbnail = nullptr;

	switch(res.group)
	{
	case ResourceGroup::Meshes:
	{
		ResourceHandle resourceHandle = rm->loadResource(options);
		MeshHandle mesh = HandleCast<Mesh>(resourceHandle);
		thumbnail = generateMesh(mesh);
		break;
	}
	default:
		//LogDebug("No resource thumbnailer was found for '%s'", cres.path.c_str());
		return;
	}

	if( !thumbnail ) return;

	Stream* fileStream = StreamCreateFromFile(AllocatorGetHeap(), res.preview, StreamMode::Write);

	ImageWriter writer;
	writer.save( thumbnail.get(), fileStream );

	StreamDestroy(fileStream);

	LogInfo("Generated thumbnail for resource '%s'", res.path.c_str());
}

//-----------------------------------//

bool ResourceThumbnailer::setupRender()
{
	RenderDevice* device = GetRenderDevice();

	RenderContext* context = device->getActiveContext();
	if( !context ) return false;

	Settings settings(ThumbSize, ThumbSize);
	renderBuffer = context->createRenderBuffer(settings);
	if( !renderBuffer ) return false;

	renderBuffer->createRenderBuffer(RenderBufferType::Depth);

	//colorTexture = renderBuffer->createRenderTexture(RenderBufferType::Color);
	//if( !colorTexture ) return false;

	renderBuffer->createRenderBuffer(RenderBufferType::Color);

	if( !renderBuffer->check() )
		return false;

	camera.reset( AllocateThis(Camera) );
	Frustum& frustum = camera->getFrustum();
	frustum.nearPlane = 0.01f;
	frustum.farPlane = 10000.0f;

	entityCamera.reset( AllocateThis(Entity) );
	entityCamera->addTransform();
	entityCamera->addComponent( camera );

#if 0
	Quaternion quat;
	quat.setToRotateAboutX( MathDegreeToRadian(180) );
	entityCamera->getTransform()->setRotation(quat);
#endif

	scene.reset( AllocateThis(Scene) );
	scene->entities.add( entityCamera );

	renderView = AllocateThis(RenderView);
	renderView->setRenderTarget(renderBuffer);
	camera->setView(renderView);

	return true;
}

//-----------------------------------//

ImagePtr ResourceThumbnailer::generateMesh(const MeshHandle& meshHandle)
{
	if( !meshHandle ) return nullptr;
	Mesh* mesh = meshHandle.Resolve();

	if( !scene && !setupRender() )
		return nullptr;

	Model* model = AllocateThis(Model);
	model->setMesh(meshHandle);
	
	Entity* entityResource = AllocateThis(Entity);
	entityResource->addTransform();
	entityResource->addComponent(model);
	scene->entities.add( entityResource );

	Transform* transResource = entityResource->getTransform().get();
	const BoundingBox& box = mesh->getBoundingVolume();
	const Vector3& center = box.getCenter();
	transResource->setPosition( Vector3(-center.x, -center.y, 0) );

	Transform* transCamera = entityCamera->getTransform().get();
	const Frustum& frustum = camera->getFrustum();
	
	float angle = std::tan(MathDegreeToRadian(frustum.fieldOfView) / 2);
	float distance = ((box.max.y - box.min.y) / 2) / angle;
	float final = -distance - ((box.max.z - box.min.z) / 2);
	
	transCamera->setPosition( Vector3(0, 0, final) );

	scene->update(0);

	ResourceManager* res = GetResourceManager();
	res->loadQueuedResources();
	res->update();

	scene->update(0);

	renderBuffer->bind();

	camera->render(scene.get());
	
	Vector2i size = renderBuffer->getSettings().getSize();

	std::vector<byte> pixels;
	renderBuffer->read(0, pixels);

	renderBuffer->unbind();

	Image* image = AllocateThis(Image);
	image->setWidth( size.x );
	image->setHeight( size.y );
	image->setPixelFormat( PixelFormat::R8G8B8A8 );
	image->setBuffer( pixels );

	scene->entities.remove( entityResource );

	return image;
}

//-----------------------------------//

NAMESPACE_EDITOR_END