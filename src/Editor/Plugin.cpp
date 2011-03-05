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
#include "Engine.h"

namespace vapor { namespace editor {

//-----------------------------------//

Plugin::Plugin( EditorFrame* frame )
	: editor( frame )
	, pluginEnabled(false)
{ }

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

void Plugin::addTool( wxAuiToolBarItem* tool, bool addToMenu )
{
	//if( tool->IsSeparator() )
		//return;

	tools.push_back( tool );

	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	toolbarCtrl->Realize();

	if( addToMenu )
	{
		editor->menuTools->Append(tool->GetId(), tool->GetLabel());
		editor->getAUI()->Update();
	}
}

//-----------------------------------//

void Plugin::removeTools()
{
	// Clean up toolbar stuff.
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	for( uint i = 0; i < tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = tools[i];
		
		int id = tool->GetId();
		toolbarCtrl->DeleteTool(id);
	}

	tools.clear();
}

//-----------------------------------//

bool Plugin::isPluginTool(int toolId) const
{
	for( uint i = 0; i < tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = tools[i];
		
		if( toolId == tool->GetId() )
			return true;
	}

	return false;
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