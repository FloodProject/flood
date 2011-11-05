/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "EventManager.h"
#include "Editor.h"
#include "EditorPlugin.h"
#include "Core/PluginManager.h"
#include "Document.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

EventManager::EventManager()
	: currentPlugin(nullptr)
	, currentTool(nullptr)
	, originalTool(nullptr)
{
	PluginManager* pm = GetEditor().getPluginManager();
	pm->onPluginEnableEvent.Connect(this, &EventManager::onPluginEnableEvent);
	pm->onPluginDisableEvent.Connect(this, &EventManager::onPluginDisableEvent);

	// Register a custom event handler so we listen to events that get
	// routed through the toolbar. This way we can intercept tool switches.

	wxAuiToolBar* toolbarCtrl = GetEditor().getToolbar();
	if(toolbarCtrl) toolbarCtrl->PushEventHandler(this);

	registerInputCallbacks();
}

//-----------------------------------//

EventManager::~EventManager()
{
	wxAuiToolBar* toolbarCtrl = GetEditor().getToolbar();
	if(toolbarCtrl) toolbarCtrl->PopEventHandler();

	InputManager* input = GetInputManager();

	// Unsubscribe from all mouse events.
	Mouse* const mouse = input->getMouse();
	mouse->onMouseMove.Disconnect(this, &EventManager::onMouseMove);
	mouse->onMouseDrag.Disconnect(this, &EventManager::onMouseDrag);
	mouse->onMouseButtonPress.Disconnect(this, &EventManager::onMousePress);
	mouse->onMouseButtonRelease.Disconnect(this, &EventManager::onMouseRelease);
	mouse->onMouseEnter.Disconnect(this, &EventManager::onMouseEnter);
	mouse->onMouseExit.Disconnect(this, &EventManager::onMouseLeave);

	// Unsubscribe from all keyboard events.
	Keyboard* const kbd = input->getKeyboard();
	kbd->onKeyPress.Disconnect(this, &EventManager::onKeyPress);
	kbd->onKeyRelease.Disconnect(this, &EventManager::onKeyRelease);
}

//-----------------------------------//

void EventManager::disconnectPluginListeners()
{
	PluginManager* pm = GetEditor().getPluginManager();
	pm->onPluginEnableEvent.Disconnect(this, &EventManager::onPluginEnableEvent);
	pm->onPluginDisableEvent.Disconnect(this, &EventManager::onPluginDisableEvent);
}

//-----------------------------------//

void EventManager::addEventListener( EditorPlugin* plugin )
{
	auto it = std::find(eventListeners.begin(), eventListeners.end(), plugin);
	
	if( it != eventListeners.end() )
		return;

	eventListeners.push_back(plugin);
}

//-----------------------------------//

void EventManager::removeEventListener( EditorPlugin* plugin )
{
	auto it = std::find(eventListeners.begin(), eventListeners.end(), plugin);

	if( it == eventListeners.end() )
		return;

	eventListeners.erase(it);
}

//-----------------------------------//

bool EventManager::TryBefore(wxEvent& event)
{
	// We are only interested in toolbar button click events.
	if( event.GetEventType() != wxEVT_COMMAND_MENU_SELECTED )
		return false;

	int id = event.GetId();

	wxAuiToolBar* toolbarCtrl = GetEditor().getToolbar();
	
	wxAuiToolBarItem* item = toolbarCtrl->FindTool(id);
	if( !item ) return false;

	if( item->GetKind() != wxITEM_RADIO )
		return false;

	EditorPlugin* plugin = (EditorPlugin*) item->GetUserData();
	
	if( !plugin )
	{
		LogDebug("Invalid tool user data");
		return false;
	}

	PluginTool* tool = plugin->findTool(item);
	if( !tool ) return false;

	setTool(tool);
	
	return false;
}

//-----------------------------------//

void EventManager::setTool(PluginTool* tool)
{
	if( !tool ) return;

	int newToolId = tool->item->GetId();

	wxAuiToolBar* toolbarCtrl = GetEditor().getToolbar();
	
	if( !toolbarCtrl->GetToolEnabled(newToolId) )
		return;

	unsetCurrentTool();

	toolbarCtrl->ToggleTool(newToolId, true);

	setCurrentTool(tool);
	setCurrentPlugin(tool->plugin);
	
	Document* document = GetEditor().getDocument();
	if( document ) document->onToolSelect(tool);
	
	currentPlugin->onToolSelect(newToolId);

	toolbarCtrl->Refresh();
}

//-----------------------------------//

void EventManager::unsetCurrentTool()
{
	if(currentPlugin)
	{
		currentPlugin->onToolUnselect(-1);
		currentPlugin = nullptr;
	}

	wxAuiToolBar* toolbarCtrl = GetEditor().getToolbar();
	toolbarCtrl->Refresh();
}

//-----------------------------------//

void EventManager::toggleTool(PluginTool* tool)
{
	if( !tool && !originalTool ) return;

	// Tool is null, restore the original tool.
	if( !tool )
	{
		setTool(originalTool);
		return;
	}

	originalTool = currentTool;
	setTool(tool);
}

//-----------------------------------//

int EventManager::getCurrentToolId()
{
	PluginTool* tool = getCurrentTool();
	if( !tool ) return wxID_INVALID;

	wxAuiToolBarItem* item = tool->item;
	if( !item ) return wxID_INVALID;

	return item->GetId();
}

//-----------------------------------//

void EventManager::onPluginEnableEvent(Plugin* plugin)
{
}

//-----------------------------------//

void EventManager::onPluginDisableEvent(Plugin* plugin)
{
	if( !plugin ) return;
	if(currentPlugin != plugin) return;

	unsetCurrentTool();
}

//-----------------------------------//

#define CALL_PLUGIN(func, ...) \
	if( currentPlugin ) currentPlugin->func(__VA_ARGS__); \
	for( size_t i = 0; i < eventListeners.size(); i++ ) { \
		EditorPlugin* plugin = eventListeners[i]; \
		if(plugin == currentPlugin) continue; \
		plugin->func(__VA_ARGS__); \
	}

#define CALL_ALL_PLUGIN(func, ...) \
	if( currentPlugin ) currentPlugin->func(__VA_ARGS__); \
	PluginManager* pm = GetEditor().getPluginManager(); \
	const std::vector<Plugin*>& plugins = pm->getPlugins(); \
	for( size_t i = 0; i < plugins.size(); i++ ) { \
		EditorPlugin* plugin = (EditorPlugin*) plugins[i]; \
		if(plugin == currentPlugin) continue; \
		plugin->func(__VA_ARGS__); \
	}

#define CALL_PLUGIN_CHECK(func, arg) \
	if(!arg) return; \
	CALL_PLUGIN(func, arg)

//-----------------------------------//

void EventManager::onDocumentCreate( Document& document )
{
	CALL_PLUGIN(onDocumentCreate, document);
}

//-----------------------------------//

void EventManager::onDocumentDestroy( Document& document )
{
	CALL_PLUGIN(onDocumentDestroy, document);
}

//-----------------------------------//

void EventManager::onDocumentSelect( Document& document )
{
	CALL_PLUGIN(onDocumentSelect, document);
}

//-----------------------------------//

void EventManager::onDocumentUnselect( Document& document )
{
	CALL_PLUGIN(onDocumentUnselect, document);
}

//-----------------------------------//

void EventManager::onEntitySelect( const EntityPtr& entity )
{
	CALL_PLUGIN_CHECK(onEntitySelect, entity);
}

//-----------------------------------//

void EventManager::onEntityUnselect( const EntityPtr& entity )
{
	CALL_PLUGIN_CHECK(onEntityUnselect, entity);
}

//-----------------------------------//

void EventManager::onComponentSelect( const ComponentPtr& component )
{
	CALL_PLUGIN_CHECK(onComponentSelect, component);
}

//-----------------------------------//

void EventManager::onComponentUnselect( const ComponentPtr& component )
{
	CALL_PLUGIN_CHECK(onComponentUnselect, component);
}

//-----------------------------------//

void EventManager::onResourceSelect( const ResourcePtr& resource )
{
	CALL_PLUGIN_CHECK(onResourceSelect, resource);
}

//-----------------------------------//

void EventManager::onResourceUnselect( const ResourcePtr& resource )
{
	CALL_PLUGIN_CHECK(onResourceUnselect, resource);
}

//-----------------------------------//

void EventManager::onMouseMove( const MouseMoveEvent& event )
{
	CALL_PLUGIN(onMouseMove, event);
}

//-----------------------------------//

void EventManager::onMouseDrag( const MouseDragEvent& event )
{
	CALL_PLUGIN(onMouseDrag, event);
}

//-----------------------------------//

void EventManager::onMousePress( const MouseButtonEvent& event )
{
	CALL_PLUGIN(onMouseButtonPress, event);
}

//-----------------------------------//

void EventManager::onMouseRelease( const MouseButtonEvent& event )
{
	CALL_PLUGIN(onMouseButtonRelease, event);
}

//-----------------------------------//

void EventManager::onMouseEnter()
{
	CALL_PLUGIN(onMouseEnter);
}

//-----------------------------------//

void EventManager::onMouseLeave()
{
	CALL_PLUGIN(onMouseLeave);
}

//-----------------------------------//

void EventManager::onKeyPress( const KeyEvent& event )
{
	CALL_PLUGIN(onKeyPress, event);
}

//-----------------------------------//

void EventManager::onKeyRelease( const KeyEvent& event )
{
	CALL_PLUGIN(onKeyRelease, event);
}

//-----------------------------------//

void EventManager::onSceneLoad( const ScenePtr& scene )
{
	CALL_ALL_PLUGIN(onSceneLoad, scene);
}

//-----------------------------------//

void EventManager::onSceneUnload( const ScenePtr& scene )
{
	CALL_ALL_PLUGIN(onSceneUnload, scene);
}

//-----------------------------------//

void EventManager::onServerConnect( const SessionPtr& scene )
{
	CALL_PLUGIN_CHECK(onServerConnect, scene);
}

//-----------------------------------//

void EventManager::onServerDisconnect( const SessionPtr& scene )
{
	CALL_PLUGIN_CHECK(onServerDisconnect, scene);
}

//-----------------------------------//

void EventManager::onUndoOperation(const UndoOperationPtr& op)
{
	CALL_PLUGIN_CHECK(onUndoOperation, op);
}

//-----------------------------------//

void EventManager::onRedoOperation(const UndoOperationPtr& op)
{
	CALL_PLUGIN_CHECK(onRedoOperation, op);
}

//-----------------------------------//

void EventManager::onSceneUpdate()
{
	CALL_PLUGIN(onSceneUpdate)
}

//-----------------------------------//

void EventManager::registerInputCallbacks()
{
	InputManager* input = GetInputManager();

	Mouse* const mouse = input->getMouse();
	mouse->onMouseMove.Connect(this, &EventManager::onMouseMove);
	mouse->onMouseDrag.Connect(this, &EventManager::onMouseDrag);
	mouse->onMouseButtonPress.Connect(this, &EventManager::onMousePress);
	mouse->onMouseButtonRelease.Connect(this, &EventManager::onMouseRelease);
	mouse->onMouseEnter.Connect(this, &EventManager::onMouseEnter);
	mouse->onMouseExit.Connect(this, &EventManager::onMouseLeave);

	Keyboard* const kbd = input->getKeyboard();
	kbd->onKeyPress.Connect(this, &EventManager::onKeyPress);
	kbd->onKeyRelease.Connect(this, &EventManager::onKeyRelease);
}

//-----------------------------------//

NAMESPACE_EDITOR_END