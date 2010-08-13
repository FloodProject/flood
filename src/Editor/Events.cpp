/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Events.h"
#include "Editor.h"
#include "Plugin.h"
#include "PluginManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

Events::Events( EditorFrame* editor )
	: editor(editor)
	, currentPlugin(nullptr)
{
	assert( editor != nullptr );
	pluginManager = editor->getPluginManager();

	registerInputCallbacks();

	pluginManager->onPluginEnableEvent +=
		fd::bind( &Events::onPluginEnableEvent, this );
	
	pluginManager->onPluginDisableEvent +=
		fd::bind( &Events::onPluginDisableEvent, this );

	// We will do some custom event handling so we get all events
	// that get routed through the toolbar. This will let us find
	// the current toolbar mode.

	wxToolBar* toolBar = editor->getToolbar();
	toolBar->PushEventHandler(this);
}

//-----------------------------------//

Events::~Events()
{
	wxToolBar* toolBar = editor->getToolbar();
	toolBar->PopEventHandler();
}

//-----------------------------------//

void Events::addEventListener( Plugin* plugin )
{
	std::vector<Plugin*>::iterator it = std::find(
		eventListeners.begin(), eventListeners.end(), plugin);
	
	if( it != eventListeners.end() )
		return;

	eventListeners.push_back(plugin);
}

//-----------------------------------//

void Events::removeEventListener( Plugin* plugin )
{
	std::vector<Plugin*>::iterator it = std::find(
		eventListeners.begin(), eventListeners.end(), plugin);

	assert( it != eventListeners.end() );

	eventListeners.erase(it);
}

//-----------------------------------//

bool Events::TryBefore(wxEvent& event)
{
	// We are only interested in toolbar button click events.
	if( event.GetEventType() != wxEVT_COMMAND_TOOL_CLICKED )
		return false;

	int id = event.GetId();

	wxToolBar* toolBar = editor->getToolbar();
	wxToolBarToolBase* tool = toolBar->FindById(id);

	if( tool->GetKind() != wxITEM_RADIO )
		return false;

	const PluginToolsMap& tools = pluginManager->getTools();
	PluginToolsMap::const_iterator it = tools.find(id);
	bool found = (it != tools.end());
	
	if( found )
		currentPlugin = (*it).second;
	else
		currentPlugin = nullptr;
	
	return false;
}

//-----------------------------------//

void Events::onPluginEnableEvent(Plugin* plugin)
{
	//if(currentPlugin == nullptr)
	//	currentPlugin = plugin;
}

//-----------------------------------//

void Events::onPluginDisableEvent(Plugin* plugin)
{
	if(currentPlugin == plugin)
		currentPlugin = nullptr;
}

//-----------------------------------//

void Events::onNodeSelect( const NodePtr& node )
{
	if( !currentPlugin )
		return;

	if(!node)
		return;

	currentPlugin->onNodeSelect(node);

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onNodeSelect(node);
}

//-----------------------------------//

void Events::onNodeUnselect( const NodePtr& node )
{
	if( !currentPlugin )
		return;

	if(!node)
		return;

	currentPlugin->onNodeUnselect(node);

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onNodeUnselect(node);
}

//-----------------------------------//

void Events::onMouseMove( const MouseMoveEvent& mve )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseMove( mve );

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onMouseMove( mve );
}

//-----------------------------------//

void Events::onMouseDrag( const MouseDragEvent& mde )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseDrag( mde );

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onMouseDrag( mde );
}

//-----------------------------------//

void Events::onMousePress( const MouseButtonEvent& mbe )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseButtonPress( mbe );

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onMouseButtonPress( mbe );
}

//-----------------------------------//

void Events::onMouseRelease( const MouseButtonEvent& mbe )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseButtonRelease( mbe );

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onMouseButtonRelease( mbe );
}

//-----------------------------------//

void Events::onMouseEnter()
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseEnter();

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onMouseEnter();
}

//-----------------------------------//

void Events::onMouseLeave()
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseLeave();

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onMouseLeave();
}

//-----------------------------------//

void Events::onSceneUpdate()
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onSceneUpdate();

	// Global event listeners.
	foreach( Plugin* plugin, eventListeners )
		plugin->onSceneUpdate();
}

//-----------------------------------//

void Events::registerInputCallbacks()
{
	Engine* engine = editor->getEngine();
	InputManager* input = engine->getInputManager();

	// Register all the mouse events.
	Mouse* const mouse = input->getMouse();

	mouse->onMouseMove +=
		fd::bind( &Events::onMouseMove, this );
	
	mouse->onMouseDrag +=
		fd::bind( &Events::onMouseDrag, this );
	
	mouse->onMouseButtonPress +=
		fd::bind( &Events::onMousePress, this );
	
	mouse->onMouseButtonRelease +=
		fd::bind( &Events::onMouseRelease, this );
	
	mouse->onMouseEnter	+=
		fd::bind( &Events::onMouseEnter, this );
	
	mouse->onMouseExit +=
		fd::bind( &Events::onMouseLeave, this );
}

//-----------------------------------//

} } // end namespaces