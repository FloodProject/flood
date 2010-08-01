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

	resourcesPage = new ResourcesPage( editor, notebookCtrl, wxID_ANY );

	bool resourcesPageAdded = notebookCtrl->AddPage( resourcesPage,
		wxT("Resources"), false, iconResources );

	if( !resourcesPageAdded )
		warn( "editor", "Could not add page to notebook" );

	notebookCtrl->Refresh();
}

//-----------------------------------//

void ResourcesPlugin::onPluginDisable()
{
	wxNotebook* notebookCtrl = editor->getNotebook();
	assert( notebookCtrl != nullptr );

	wxImageList* imageList = notebookCtrl->GetImageList();
	assert( imageList != nullptr );

	//imageList->Remove(iconResources);

	int pageCount = notebookCtrl->GetPageCount();
	
	for( int i = 0; i < pageCount; i++ )
	{
		wxWindow* window = notebookCtrl->GetPage(i);
		
		if( window->GetId() == resourcesPage->GetId() )
		{
			notebookCtrl->DeletePage(i);
			break;
		}
	}
}

//-----------------------------------//

} } // end namespaces