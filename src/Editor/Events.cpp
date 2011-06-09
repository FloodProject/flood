/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
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
	pluginManager = editor->getPluginManager();

	pluginManager->onPluginEnableEvent.Connect(this, &Events::onPluginEnableEvent);
	pluginManager->onPluginDisableEvent.Connect(this, &Events::onPluginDisableEvent);

	// We will do some custom event handling so we get all events
	// that get routed through the toolbar. This will let us find
	// the current toolbar mode.

	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	if(toolbarCtrl) toolbarCtrl->PushEventHandler(this);
	//editor->PushEventHandler(this);

	registerInputCallbacks();
}

//-----------------------------------//

Events::~Events()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	if(toolbarCtrl) toolbarCtrl->PopEventHandler();
	//editor->PopEventHandler();

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

	// Unsubscribe from all keyboard events.
	Keyboard* const kbd = input->getKeyboard();
	kbd->onKeyPress.Disconnect(this, &Events::onKeyPress);
	kbd->onKeyRelease.Disconnect(this, &Events::onKeyRelease);
}

//-----------------------------------//

void Events::disconnectPluginListeners()
{
	pluginManager->onPluginEnableEvent.Disconnect(this, &Events::onPluginEnableEvent);
	pluginManager->onPluginDisableEvent.Disconnect(this, &Events::onPluginDisableEvent);
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
	if( event.GetEventType() != wxEVT_COMMAND_MENU_SELECTED )
		return false;

	int id = event.GetId();

	//wxObject* object = event.GetEventObject();

	//if( !object )
	//	return false;

	//if( !object->IsKindOf(&wxAuiToolBarItem::ms_classInfo) )
	//	return false;

	wxAuiToolBar* toolbarCtrl = editor->getToolbar();
	wxAuiToolBarItem* tool = toolbarCtrl->FindTool(id);

	if( !tool ) return false;

	if( tool->GetKind() != wxITEM_RADIO )
		return false;

	toolbarCtrl->ToggleTool(id, true);

	const PluginToolsMap& tools = pluginManager->getTools();
	PluginToolsMap::const_iterator it = tools.find(id);
	
	if( it == tools.end() )
	{
		toolId = 0;
		currentPlugin = nullptr;
	
		return false;
	}
	
	int newToolId = it->first;

	if(newToolId != toolId)
	{
		toolId = newToolId;

		currentPlugin->onToolUnselect( toolId );
		currentPlugin = (*it).second;
		currentPlugin->onToolSelect( toolId );
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

#define CALL_PLUGIN(func, ...)							\
	if( currentPlugin )									\
		currentPlugin->func(__VA_ARGS__);				\
														\
	for( uint i = 0; i < eventListeners.size(); i++ )	\
	{													\
		Plugin* plugin = eventListeners[i];				\
		if(plugin == currentPlugin) continue;			\
		plugin->func(__VA_ARGS__);						\
	}

#define CALL_PLUGIN_CHECK(func, arg)					\
	if(!arg) return;									\
	CALL_PLUGIN(func, arg)

//-----------------------------------//

void Events::onDocumentSelect( Document& document )
{
	CALL_PLUGIN(onDocumentSelect, document);
}

//-----------------------------------//

void Events::onDocumentUnselect( Document& document )
{
	CALL_PLUGIN(onDocumentUnselect, document);
}

//-----------------------------------//

void Events::onEntitySelect( const EntityPtr& entity )
{
	CALL_PLUGIN_CHECK(onEntitySelect, entity);
}

//-----------------------------------//

void Events::onEntityUnselect( const EntityPtr& entity )
{
	CALL_PLUGIN_CHECK(onEntityUnselect, entity);
}

//-----------------------------------//

void Events::onComponentSelect( const ComponentPtr& component )
{
	CALL_PLUGIN_CHECK(onComponentSelect, component);
}

//-----------------------------------//

void Events::onComponentUnselect( const ComponentPtr& component )
{
	CALL_PLUGIN_CHECK(onComponentUnselect, component);
}

//-----------------------------------//

void Events::onResourceSelect( const ResourcePtr& resource )
{
	CALL_PLUGIN_CHECK(onResourceSelect, resource);
}

//-----------------------------------//

void Events::onResourceUnselect( const ResourcePtr& resource )
{
	CALL_PLUGIN_CHECK(onResourceUnselect, resource);
}

//-----------------------------------//

void Events::onMouseMove( const MouseMoveEvent& event )
{
	CALL_PLUGIN(onMouseMove, event);
}

//-----------------------------------//

void Events::onMouseDrag( const MouseDragEvent& event )
{
	CALL_PLUGIN(onMouseDrag, event);
}

//-----------------------------------//

void Events::onMousePress( const MouseButtonEvent& event )
{
	CALL_PLUGIN(onMouseButtonPress, event);
}

//-----------------------------------//

void Events::onMouseRelease( const MouseButtonEvent& event )
{
	CALL_PLUGIN(onMouseButtonRelease, event);
}

//-----------------------------------//

void Events::onMouseEnter()
{
	CALL_PLUGIN(onMouseEnter);
}

//-----------------------------------//

void Events::onMouseLeave()
{
	CALL_PLUGIN(onMouseLeave);
}

//-----------------------------------//

void Events::onKeyPress( const KeyEvent& event )
{
	CALL_PLUGIN(onKeyPress, event);
}

//-----------------------------------//

void Events::onKeyRelease( const KeyEvent& event )
{
	CALL_PLUGIN(onKeyRelease, event);
}

//-----------------------------------//

void Events::onSceneLoad( const ScenePtr& scene )
{
	CALL_PLUGIN_CHECK(onSceneLoad, scene);
}

//-----------------------------------//

void Events::onSceneUpdate()
{
	CALL_PLUGIN(onSceneUpdate)
}

//-----------------------------------//

void Events::registerInputCallbacks()
{
	Engine* engine = editor->getEngine();
	InputManager* input = engine->getInputManager();

	Mouse* const mouse = input->getMouse();
	mouse->onMouseMove.Connect(this, &Events::onMouseMove);
	mouse->onMouseDrag.Connect(this, &Events::onMouseDrag);
	mouse->onMouseButtonPress.Connect(this, &Events::onMousePress);
	mouse->onMouseButtonRelease.Connect(this, &Events::onMouseRelease);
	mouse->onMouseEnter.Connect(this, &Events::onMouseEnter);
	mouse->onMouseExit.Connect(this, &Events::onMouseLeave);

	Keyboard* const kbd = input->getKeyboard();
	kbd->onKeyPress.Connect(this, &Events::onKeyPress);
	kbd->onKeyRelease.Connect(this, &Events::onKeyRelease);
}

//-----------------------------------//

} } // end namespaces