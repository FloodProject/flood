/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "CameraPlugin.h"
#include "CameraControls.h"
#include "Editor.h"
#include "Viewframe.h"

namespace vapor { namespace editor {

//-----------------------------------//

CameraPlugin::CameraPlugin( EditorFrame* frame )
	: Plugin(frame)
	, cameraControls(nullptr)
{ }

//-----------------------------------//

PluginMetadata CameraPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Camera Controls";
	metadata.description = "Provides advanced camera control";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void CameraPlugin::onPluginEnable()
{
	Viewframe* viewframe = editor->getMainViewframe();
	cameraControls = new CameraControls(viewframe);

	wxSizerFlags sizerFlags(0);
	sizerFlags.Expand().Border(wxTOP, 2);

	wxSizer* viewSizer = viewframe->getSizer();
	viewSizer->Add( cameraControls, sizerFlags );
	viewSizer->Layout();
}

//-----------------------------------//

void CameraPlugin::onPluginDisable()
{
	Viewframe* viewframe = editor->getMainViewframe();
	viewframe->RemoveChild(cameraControls);

	wxSizer* viewSizer = viewframe->getSizer();
	bool found = viewSizer->Detach(cameraControls);
	assert( found == true );
	viewSizer->Layout();
		
	cameraControls->Destroy();
}

//-----------------------------------//

} } // end namespaces