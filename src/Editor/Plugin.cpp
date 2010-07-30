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

bool Plugin::isPluginEnabled() const
{
	return pluginEnabled;
}

//-----------------------------------//

} } // end namespaces