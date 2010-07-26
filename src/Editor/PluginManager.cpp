/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PluginManager.h"
#include "Plugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

PluginManager::PluginManager(EditorFrame* frame)
	: currentTool(nullptr)
	, editorFrame(frame)
{
	registerInputCallbacks();
}

//-----------------------------------//

PluginManager::~PluginManager()
{
	foreach( const Plugin* plugin, plugins )
		delete plugin;
}

//-----------------------------------//

void PluginManager::registerPlugin( Plugin* plugin )
{
	if( !plugin )
		return;

	plugins.push_back(plugin);
}

//-----------------------------------//

void PluginManager::enablePlugin( Plugin* plugin )
{
	if( !plugin )
		return;
	
	PluginMetadata metadata = plugin->getMetadata();
	info( "editor", "Enabling plugin: %s", metadata.name.c_str() );

	plugin->pluginEnabled = true;

	wxToolBar* toolBar = editorFrame->GetToolBar();
	plugin->onPluginEnable(toolBar);

	toolBar->Realize();
}

//-----------------------------------//

void PluginManager::disablePlugin( Plugin* plugin )
{
	if( !plugin )
		return;

	PluginMetadata metadata = plugin->getMetadata();
	info( "editor", "Disabling plugin: %s", metadata.name.c_str() );

	plugin->pluginEnabled = false;
	
	wxToolBar* toolBar = editorFrame->GetToolBar();
	plugin->onPluginDisable(toolBar);

	toolBar->Realize();
}

//-----------------------------------//

void PluginManager::onNodeSelect( const NodePtr& node )
{
	if( !currentTool )
		return;

	if(!node)
		return;

	currentTool->onNodeSelect(node);
}

//-----------------------------------//

void PluginManager::onNodeUnselect( const NodePtr& node )
{
	if( !currentTool )
		return;

	if(!node)
		return;

	currentTool->onNodeUnselect(node);
}

//-----------------------------------//

void PluginManager::onMouseMove( const MouseMoveEvent& mve )
{
	if( !currentTool )
		return;
	
	currentTool->onMouseMove( mve );
}

//-----------------------------------//

void PluginManager::onMouseDrag( const MouseDragEvent& mde )
{
	if( !currentTool )
		return;
	
	currentTool->onMouseDrag( mde );
}

//-----------------------------------//

void PluginManager::onMousePress( const MouseButtonEvent& mbe )
{
	if( !currentTool )
		return;
	
	currentTool->onMouseButtonPress( mbe );
}

//-----------------------------------//

void PluginManager::onMouseRelease( const MouseButtonEvent& mbe )
{
	if( !currentTool )
		return;
	
	currentTool->onMouseButtonRelease( mbe );
}

//-----------------------------------//

void PluginManager::onMouseEnter()
{
	if( !currentTool )
		return;
	
	currentTool->onMouseEnter();
}

//-----------------------------------//

void PluginManager::onMouseLeave()
{
	if( !currentTool )
		return;
	
	currentTool->onMouseLeave();
}

//-----------------------------------//

void PluginManager::registerInputCallbacks()
{
	Engine* engine = editorFrame->getEngine();

	// Register all the mouse events.
	Mouse* const mouse = engine->getInputManager()->getMouse();

	mouse->onMouseMove += fd::bind( &PluginManager::onMouseMove, this );
	mouse->onMouseDrag += fd::bind( &PluginManager::onMouseDrag, this );
	mouse->onMouseButtonPress += fd::bind( &PluginManager::onMousePress, this );
	mouse->onMouseButtonRelease += fd::bind( &PluginManager::onMouseRelease, this );
	mouse->onMouseEnter	+= fd::bind( &PluginManager::onMouseEnter, this );
	mouse->onMouseExit += fd::bind( &PluginManager::onMouseLeave, this );
}

//-----------------------------------//

} } // end namespaces