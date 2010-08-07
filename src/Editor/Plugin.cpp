/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Plugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

Plugin::Plugin( EditorFrame* frame )
	: editor( frame )
	, pluginEnabled(false)
{
	assert( editor != nullptr );

	engine = editor->getEngine();
	viewframe = editor->getMainViewframe();
}

//-----------------------------------//

Plugin::~Plugin()
{

}

//-----------------------------------//

void Plugin::doPluginDisable()
{
	onPluginDisable();
	removeTools();
}

//-----------------------------------//

void Plugin::addTool( wxToolBarToolBase* tool )
{
	tools.push_back( tool );

	wxToolBar* toolBar = editor->getToolbar();
	toolBar->Realize();
}

//-----------------------------------//

void Plugin::removeTools()
{
	// Clean up toolbar stuff.
	wxToolBar* toolBar = editor->getToolbar();

	foreach( wxToolBarToolBase* tool, tools )
	{
		int id = tool->GetId();
		toolBar->DeleteTool(id);
	}

	tools.clear();
}

//-----------------------------------//

void Plugin::removePage( wxWindow* page )
{
	if( !page )
		return;

	wxNotebook* notebookCtrl = editor->getNotebook();
	assert( notebookCtrl != nullptr );

	wxImageList* imageList = notebookCtrl->GetImageList();
	assert( imageList != nullptr );

	//imageList->Remove(iconPage);

	int pageCount = notebookCtrl->GetPageCount();
	
	for( int i = 0; i < pageCount; i++ )
	{
		wxWindow* window = notebookCtrl->GetPage(i);
		
		if( window->GetId() == page->GetId() )
		{
			notebookCtrl->DeletePage(i);
			break;
		}
	}
}

//-----------------------------------//

bool Plugin::isPluginEnabled() const
{
	return pluginEnabled;
}

//-----------------------------------//

} } // end namespaces