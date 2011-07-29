/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ResourceThumbnailer
{
	ResourceThumbnailer();

public:

	// Generate thumbnail.
	ImagePtr generateThumbnail(Mesh* mesh);

protected:

	// Sets up the render buffers.
	void setupRender();

	ScenePtr scene;
	EntityPtr entityCamera;
	CameraPtr camera;
	RenderView* renderView;

	TexturePtr depthTexture;
	TexturePtr colorTexture;
	RenderBuffer* renderBuffer;
};

//-----------------------------------//

NAMESPACE_EDITOR_END