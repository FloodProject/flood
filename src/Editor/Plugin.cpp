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
{ }

//-----------------------------------//

void Plugin::doPluginDisable()
{
	Engine* engine = editor->getEngine();
	const ScenePtr& scene = engine->getSceneManager();

	if(scene)
		onSceneUnload(scene);

	onPluginDisable();
	removeTools();
}

//-----------------------------------//

void Plugin::addTool( wxAuiToolBarItem* tool )
{
	//if( tool->IsSeparator() )
		//return;

	tools.push_back( tool );

	wxAuiToolBar* toolBar = editor->getToolbar();
	toolBar->Realize();
}

//-----------------------------------//

void Plugin::removeTools()
{
	// Clean up toolbar stuff.
	wxAuiToolBar* toolBar = editor->getToolbar();

	for( uint i = 0; i < tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = tools[i];
		
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

	wxAuiNotebook* notebookCtrl = editor->getNotebook();

	int pageCount = notebookCtrl->GetPageCount();
	
	for( int i = 0; i < pageCount; i++ )
	{
		wxWindow* window = notebookCtrl->GetPage(i);
		
		if( window->GetId() == page->GetId() )
		{
			notebookCtrl->RemovePage(i);
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