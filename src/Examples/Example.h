/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <vapor/Framework.h>
#include <vapor/Headers.h>

using namespace vapor;

//-----------------------------------//

class Example : public vapor::Framework
{
public:

	Example(const char** argv = nullptr);

	void onPageLoading(const PageEvent& event);

protected:

	// Initializes the app.
	virtual void onInit();	

	// Sets up all the needed resources.
	virtual void onSetupResources();

	// Called to set up the scene.
	virtual void onSetupScene();

	// Called to update stuff.
	virtual void onUpdate( float delta );

	// Renders the scene.
	virtual void onRender();

	// Callback functions.
	virtual void onKeyPressed( const KeyEvent& );
	virtual void onButtonPressed( const MouseButtonEvent& );

private:
	
	Window* window;
	RenderView* view;
	CameraPtr camera;

	LabelPtr labelFPS;
	TerrainPtr nodeTerrain;

	EntityPtr nodeFBO;
	RenderView* viewportFBO;
	TexturePtr textureFBO;
	RenderBuffer* bufferFBO;
};

#include "Core/Memory.h"
#include "Core/Concurrency.h"
#include "Core/Archive.h"

//-----------------------------------//

int main(int argc, const char** argv)
{
	Example example;
	//example.run();

	Archive* archive = ArchiveCreateFromZip( AllocatorGetDefault(), "Teste.zip" );
	
	std::vector<Path> files;
	//ArchiveEnumerateFiles(archive, files);

	for(size_t i = 0; i < files.size(); i++)
		printf("ZIP file entry: %s\n", files[i].c_str());

	ArchiveDestroy(archive, AllocatorGetDefault());

	return EXIT_SUCCESS;
}