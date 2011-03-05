/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ResourcesPlugin.h"
#include "ResourcesPane.h"
#include "ResourcesBrowser.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Pipeline/ResourceProcessor.h"

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesPlugin::ResourcesPlugin( EditorFrame* frame )
	: Plugin(frame)
	, resourcesPage(nullptr)
	, resourcesBrowser(nullptr)
{ }

//-----------------------------------//

ResourcesPlugin::~ResourcesPlugin()
{
	delete resourcesPage;
	delete resourcesBrowser;
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
	resourcesPage = new ResourcesPage(editor);
	resourcesPage->SetSize(300, 200);

	wxBitmap iconPackage = wxMEMORY_BITMAP(package);

	wxAuiPaneInfo pane;
	pane.Caption("Resources").Left().Dock().Icon(iconPackage);

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

	ResourceProcessor::referenceProcessors();
	ResourceProcessor::registerProcessors();
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	editor->getAUI()->DetachPane(resourcesPage);
	editor->getAUI()->Update();

	delete resourcesBrowser;
	resourcesBrowser = nullptr;

	delete resourcesPage;
	resourcesPage = nullptr;
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