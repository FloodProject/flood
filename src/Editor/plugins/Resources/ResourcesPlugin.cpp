/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ResourcesPlugin.h"
#include "ResourcesPane.h"
#include "ResourcesBrowser.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Pipeline/ResourceProcessor.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CHILD_CLASS(ResourcesPlugin, Plugin)
REFLECT_CLASS_END()

//-----------------------------------//

ResourcesPlugin::ResourcesPlugin()
	: Plugin()
	, resourcesPage(nullptr)
	, resourcesBrowser(nullptr)
{ }

//-----------------------------------//

ResourcesPlugin::~ResourcesPlugin()
{
	Deallocate(resourcesPage);
	Deallocate(resourcesBrowser);
}

//-----------------------------------//

PluginMetadata ResourcesPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Resources";
	metadata.description = "Provides resource handling features";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void ResourcesPlugin::onPluginEnable()
{
	resourcesPage = Allocate(ResourcesPage, AllocatorGetHeap(), editor);
	resourcesPage->SetSize(300, 200);

	wxBitmap iconPackage = wxMEMORY_BITMAP(package);
	wxAuiPaneInfo pane;
	pane.Caption("Resources").Left().Dock().Icon(iconPackage).PaneBorder(false);
	editor->getAUI()->AddPane(resourcesPage, pane);
	editor->getAUI()->Update();

#if 0
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	if(toolbarCtrl)
	{
		resourcesBrowserButton = toolbarCtrl->AddTool( wxID_ANY, "Resources Browser", iconPackage );
		addTool( resourcesBrowserButton );
		
		toolbarCtrl->EnableTool(resourcesBrowserButton->GetId(), false);
		
		toolbarCtrl->Bind( wxEVT_COMMAND_TOOL_CLICKED,
			&ResourcesPlugin::onBrowserButtonClick,
			this, resourcesBrowserButton->GetId() );
	}

	resourcesBrowser = new ResourcesBrowser(editor, editor);
#endif

	//Pipeline::ReferenceProcessors();
	Pipeline::Init();
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	editor->getAUI()->DetachPane(resourcesPage);
	editor->getAUI()->Update();

	Deallocate(resourcesBrowser);
	resourcesBrowser = nullptr;

	Deallocate(resourcesPage);
	resourcesPage = nullptr;

	Pipeline::Cleanup();
}

//-----------------------------------//

void ResourcesPlugin::onBrowserButtonClick(wxCommandEvent& event)
{
#if 0
	static bool scanned = false;

	if(!scanned)
	{
		resourcesBrowser->scanFiles();
		resourcesBrowser->setupImages();
		scanned = true;
	}

	resourcesBrowser->Show();
#endif
}

//-----------------------------------//

} } // end namespaces