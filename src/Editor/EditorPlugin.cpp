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
#include "EventManager.h"

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

	// Unsubscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->removeEventListener(this);

	removeTools();
}

//-----------------------------------//

PluginTool* EditorPlugin::findTool( wxAuiToolBarItem* tool )
{
	if( !tool ) return nullptr;
	
	int toolId = tool->GetId();
	return findToolById(toolId);
}

//-----------------------------------//

PluginTool* EditorPlugin::findToolById( int toolId )
{
	for( size_t i = 0; i < tools.size(); i++ )
	{
		PluginTool& tool = tools[i];
		
		wxAuiToolBarItem* item = tool.item;
		if( !item ) continue;

		if(item->GetId() == toolId) return &tool;
	}

	return nullptr;
}

//-----------------------------------//

void EditorPlugin::addTool( PluginTool& tool, bool addToMenu )
{
	tool.item->SetUserData((long)this);
	tool.plugin = this;

	tools.push_back( tool );

	wxAuiToolBar* toolbar = tool.toolbar;
	
	if( toolbar )
	{
		toolbar->Hide();
		toolbar->Realize();
	}
	
	addToMenu = true;

	if( addToMenu && tool.item->GetKind() == wxITEM_NORMAL )
	{
		wxAuiToolBarItem* item = tool.item;
		editor->menuTools->Append(item->GetId(), item->GetLabel());

		wxAuiToolBar* toolbarCtrl = editor->getToolbar();
		toolbarCtrl->Realize();

		//editor->getAUI()->Update();
	}
}

//-----------------------------------//

void EditorPlugin::addTool( wxAuiToolBarItem* item, bool addToMenu )
{
	PluginTool tool;
	tool.item = item;

	addTool(tool);
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
	//editor->getAUI()->Update();

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

bool EditorPlugin::isToolSelected() const
{
	EventManager* events = GetEditor().getEventManager();
	
	int id = events->getCurrentToolId();
	if( id == wxID_INVALID ) return false;

	return hasTool(id);
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