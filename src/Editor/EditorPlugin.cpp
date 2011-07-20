/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "EditorPlugin.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(EditorPlugin, Plugin)
REFLECT_CLASS_END()

//-----------------------------------//

EditorPlugin::EditorPlugin()
	: editor(nullptr)
{
	editor = &GetEditor();
}

//-----------------------------------//

EditorPlugin::~EditorPlugin()
{
}

//-----------------------------------//

void EditorPlugin::doPluginDisable()
{
	Plugin::doPluginDisable();
	removeTools();
}

//-----------------------------------//

PluginTool* EditorPlugin::findTool( wxAuiToolBarItem* tool )
{
	for( size_t i = 0; i < tools.size(); i++ )
	{
		PluginTool& p = tools[i];
		if(p.item == tool) return &p;
	}

	return nullptr;
}

//-----------------------------------//

void EditorPlugin::addTool( const PluginTool& pluginTool, bool addToMenu )
{
	//if( tool->IsSeparator() ) return;

	tools.push_back( pluginTool );

	wxAuiToolBar* toolbar = pluginTool.toolbar;
	
	if( toolbar )
	{
		toolbar->Hide();
		toolbar->Realize();
	}

	if( addToMenu )
	{
		wxAuiToolBarItem* tool = pluginTool.item;
		editor->menuTools->Append(tool->GetId(), tool->GetLabel());

		wxAuiToolBar* toolbarCtrl = editor->getToolbar();
		toolbarCtrl->Realize();

		editor->getAUI()->Update();
	}
}

//-----------------------------------//

void EditorPlugin::addTool( wxAuiToolBarItem* tool, bool addToMenu )
{
	PluginTool pt;
	pt.item = tool;

	addTool(pt);
}

//-----------------------------------//

void PluginTool::setToolbar(wxAuiToolBar* tb)
{
	tb->Realize();
	toolbar = tb;
}

//-----------------------------------//

void EditorPlugin::removeTools()
{
	// Clean up toolbar stuff.
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	for( size_t i = 0; i < tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = tools[i].item;
		int id = tool->GetId();
		toolbarCtrl->DeleteTool(id);
	}

	toolbarCtrl->Realize();

	tools.clear();
}

//-----------------------------------//

bool EditorPlugin::hasTool(int toolId) const
{
	for( size_t i = 0; i < tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = tools[i].item;
		if( toolId == tool->GetId() ) return true;
	}

	return false;
}

//-----------------------------------//

void EditorPlugin::removePage( wxWindow* page )
{
	if( !page ) return;

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

NAMESPACE_EDITOR_END