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

REFLECT_ABSTRACT_CHILD_CLASS(EditorExtension, Extension)
REFLECT_CLASS_END()

EditorExtension::EditorExtension()
	: plugin(nullptr)
{

}

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(ToolExtension, EditorExtension)
REFLECT_CLASS_END()

ToolExtension::ToolExtension()
	: toolbar(nullptr)
	, item(nullptr)
{

}

//-----------------------------------//

ExtensionMetadata* ToolExtension::getMetadata()
{
	static ExtensionMetadata s_ToolExtension =
	{
		"Tool",
		"Extends the editor with tools."
	};

	return &s_ToolExtension;
}

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(DocumentExtension, EditorExtension)
REFLECT_CLASS_END()

DocumentExtension::DocumentExtension()
{

}

//-----------------------------------//

ExtensionMetadata* DocumentExtension::getMetadata()
{
	static ExtensionMetadata s_DocumentExtension =
	{
		"Document",
		"Extends the editor with documents."
	};

	return &s_DocumentExtension;
}

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

ToolExtension* EditorPlugin::findTool( wxAuiToolBarItem* tool )
{
	if( !tool ) return nullptr;
	
	int toolId = tool->GetId();
	return findToolById(toolId);
}

//-----------------------------------//

ToolExtension* EditorPlugin::findToolById( int toolId )
{
	for( size_t i = 0; i < tools.size(); i++ )
	{
		ToolExtension& tool = tools[i];
		
		wxAuiToolBarItem* item = tool.item;
		if( !item ) continue;

		if(item->GetId() == toolId) return &tool;
	}

	return nullptr;
}

//-----------------------------------//

void EditorPlugin::addTool( ToolExtension& tool, bool addToMenu )
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
	ToolExtension tool;
	tool.item = item;

	addTool(tool);
}

//-----------------------------------//

void ToolExtension::setToolbar(wxAuiToolBar* tb)
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