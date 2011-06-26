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

namespace vapor { namespace editor {

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

} } // end namespaces