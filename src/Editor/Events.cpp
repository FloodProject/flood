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
	, toolId(0)
{
	assert( editor != nullptr );
	pluginManager = editor->getPluginManager();

	registerInputCallbacks();

	pluginManager->onPluginEnableEvent.Connect(this, &Events::onPluginEnableEvent);
	pluginManager->onPluginDisableEvent.Connect(this, &Events::onPluginDisableEvent);

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

	pluginManager->onPluginEnableEvent.Disconnect(this, &Events::onPluginEnableEvent);
	pluginManager->onPluginDisableEvent.Disconnect(this, &Events::onPluginDisableEvent);

	Engine* engine = editor->getEngine();
	InputManager* input = engine->getInputManager();

	// Unsubscribe from all mouse events.
	Mouse* const mouse = input->getMouse();
	mouse->onMouseMove.Disconnect(this, &Events::onMouseMove);
	mouse->onMouseDrag.Disconnect(this, &Events::onMouseDrag);
	mouse->onMouseButtonPress.Disconnect(this, &Events::onMousePress);
	mouse->onMouseButtonRelease.Disconnect(this, &Events::onMouseRelease);
	mouse->onMouseEnter.Disconnect(this, &Events::onMouseEnter);
	mouse->onMouseExit.Disconnect(this, &Events::onMouseLeave);
}

//-----------------------------------//

void Events::addEventListener( Plugin* plugin )
{
	std::vector<Plugin*>::iterator it;
	it = std::find(eventListeners.begin(), eventListeners.end(), plugin);
	
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
	
	if( it != tools.end() )
	{
		toolId = it->first;

		currentPlugin->onToolUnselect( toolId );
		currentPlugin = (*it).second;
		currentPlugin->onToolSelect( toolId );
	}
	else
	{
		toolId = 0;
		currentPlugin = nullptr;
	}
	
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

void Events::onEntitySelect( const EntityPtr& node )
{
	if( !currentPlugin )
		return;

	if(!node)
		return;

	currentPlugin->onEntitySelect(node);

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onEntitySelect(node);
	}
}

//-----------------------------------//

void Events::onEntityUnselect( const EntityPtr& node )
{
	if( !currentPlugin )
		return;

	if(!node)
		return;

	currentPlugin->onEntityUnselect(node);

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onEntityUnselect(node);
	}
}

//-----------------------------------//

void Events::onComponentSelect( const ComponentPtr& component )
{
	if( !currentPlugin )
		return;

	if(!component)
		return;

	currentPlugin->onComponentSelect(component);

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onComponentSelect(component);
	}
}

//-----------------------------------//

void Events::onComponentUnselect( const ComponentPtr& component )
{
	if( !currentPlugin )
		return;

	if(!component)
		return;

	currentPlugin->onComponentUnselect(component);

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onComponentUnselect(component);
	}
}


//-----------------------------------//

void Events::onMouseMove( const MouseMoveEvent& mve )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseMove( mve );

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onMouseMove( mve );
	}
}

//-----------------------------------//

void Events::onMouseDrag( const MouseDragEvent& mde )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseDrag( mde );

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onMouseDrag( mde );
	}
}

//-----------------------------------//

void Events::onMousePress( const MouseButtonEvent& mbe )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseButtonPress( mbe );

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onMouseButtonPress( mbe );
	}
}

//-----------------------------------//

void Events::onMouseRelease( const MouseButtonEvent& mbe )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseButtonRelease( mbe );

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onMouseButtonRelease( mbe );
	}
}

//-----------------------------------//

void Events::onMouseEnter()
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseEnter();

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onMouseEnter();
	}
}

//-----------------------------------//

void Events::onMouseLeave()
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onMouseLeave();

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onMouseLeave();
	}
}

//-----------------------------------//

void Events::onSceneLoad( const ScenePtr& newScene )
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onSceneLoad(newScene);

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onSceneLoad(newScene);
	}
}

//-----------------------------------//

void Events::onSceneUpdate()
{
	if( !currentPlugin )
		return;
	
	currentPlugin->onSceneUpdate();

	// Global event listeners.
	for( uint i = 0; i < eventListeners.size(); i++ )
	{
		Plugin* plugin = eventListeners[i];
		if(plugin == currentPlugin) continue;
		plugin->onSceneUpdate();
	}
}

//-----------------------------------//

void Events::registerInputCallbacks()
{
	Engine* engine = editor->getEngine();
	InputManager* input = engine->getInputManager();

	// Register all the mouse events.
	Mouse* const mouse = input->getMouse();

	mouse->onMouseMove.Connect(this, &Events::onMouseMove);
	mouse->onMouseDrag.Connect(this, &Events::onMouseDrag);
	mouse->onMouseButtonPress.Connect(this, &Events::onMousePress);
	mouse->onMouseButtonRelease.Connect(this, &Events::onMouseRelease);
	mouse->onMouseEnter.Connect(this, &Events::onMouseEnter);
	mouse->onMouseExit.Connect(this, &Events::onMouseLeave);
}

//-----------------------------------//

} } // end namespaces