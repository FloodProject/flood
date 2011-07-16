/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Plugin.h"
#include "Editor.h"
#include "Engine/Engine.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(Plugin, Object)
REFLECT_CLASS_END()

//-----------------------------------//

Plugin::Plugin()
	: enabled(false)
	, editor(nullptr)
{

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

PluginTool* Plugin::findTool( wxAuiToolBarItem* tool )
{
	for( size_t i = 0; i < tools.size(); i++ )
	{
		PluginTool& p = tools[i];
		if(p.item == tool) return &p;
	}

	return nullptr;
}

//-----------------------------------//

void Plugin::addTool( const PluginTool& pluginTool, bool addToMenu )
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

void Plugin::addTool( wxAuiToolBarItem* tool, bool addToMenu )
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

void Plugin::removeTools()
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

bool Plugin::hasTool(int toolId) const
{
	for( size_t i = 0; i < tools.size(); i++ )
	{
		wxAuiToolBarItem* tool = tools[i].item;
		if( toolId == tool->GetId() ) return true;
	}

	return false;
}

//-----------------------------------//

void Plugin::removePage( wxWindow* page )
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

bool Plugin::isEnabled() const
{
	return enabled;
}

//-----------------------------------//

NAMESPACE_EDITOR_END