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
	wxNotebook* notebookCtrl = editor->getNotebook();
	assert( notebookCtrl != nullptr );

	wxImageList* imageList = notebookCtrl->GetImageList();
	assert( imageList != nullptr );

	wxBitmap iconPackage = wxMEMORY_BITMAP(package);
	iconResources = imageList->Add(iconPackage);

	resourcesPage = new ResourcesPage( editor, notebookCtrl );

	bool resourcesPageAdded = notebookCtrl->AddPage( resourcesPage,
		wxEmptyString/*wxT("Resources")*/, false, iconResources );

	if( !resourcesPageAdded )
		Log::warn( "Could not add page to notebook" );

	notebookCtrl->Refresh();

	wxToolBar* toolBar = editor->getToolbar();

	resourcesBrowserButton = toolBar->AddTool(
		wxID_ANY, "Resources Browser", iconPackage );
	addTool( resourcesBrowserButton );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&ResourcesPlugin::onBrowserButtonClick,
		this, resourcesBrowserButton->GetId() );

	resourcesBrowser = new ResourcesBrowser( editor );
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	removePage( resourcesPage );
}

//-----------------------------------//

void ResourcesPlugin::onBrowserButtonClick(wxCommandEvent& event)
{
	resourcesBrowser->scanFiles();
	resourcesBrowser->Show();
}

//-----------------------------------//

} } // end namespaces