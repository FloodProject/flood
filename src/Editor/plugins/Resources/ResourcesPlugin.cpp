/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ResourcesPlugin.h"
#include "ResourcesPage.h"
#include "ResourcesBrowser.h"
#include "Editor.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

ResourcesPlugin::ResourcesPlugin( EditorFrame* frame )
	: Plugin(frame)
	, resourcesPage(nullptr)
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
	metadata.description = "Provides a page with the known resources";
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
	pane.Caption("Resources").Left().Dock().Hide().Icon(iconPackage);

	editor->getAUI()->AddPane(resourcesPage, pane);
	editor->getAUI()->Update();

	wxAuiToolBar* toolBar = editor->getToolbar();

	if(toolBar)
	{
		resourcesBrowserButton = toolBar->AddTool( wxID_ANY, "Resources Browser", iconPackage );
		addTool( resourcesBrowserButton );
		
		toolBar->EnableTool(resourcesBrowserButton->GetId(), false);
		
		toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
			&ResourcesPlugin::onBrowserButtonClick,
			this, resourcesBrowserButton->GetId() );
	}

	resourcesBrowser = new ResourcesBrowser(editor, editor);
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
	static bool scanned = false;

	if(!scanned)
	{
		resourcesBrowser->scanFiles();
		resourcesBrowser->setupImages();
		scanned = true;
	}

	resourcesBrowser->Show();
}

//-----------------------------------//

} } // end namespaces