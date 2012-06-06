/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SelectionPlugin.h"
#include "SelectionManager.h"
#include "SelectionOperation.h"
#include "EventManager.h"
#include "UndoManager.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Extensions/Scene/SceneDocument.h"

#include <algorithm>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(SelectionPlugin, EditorPlugin)
REFLECT_CLASS_END()

namespace SelectionTool
{
	enum Enum
	{
		Select = wxID_HIGHEST + 42
	};
}

//-----------------------------------//

SelectionPlugin::SelectionPlugin()
{

}

//-----------------------------------//

PluginMetadata SelectionPlugin::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = PLUGIN_SELECTION;
	metadata.description = "Provides selection services.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 250;

	return metadata;
}

//-----------------------------------//

class ToolbarHack : public wxAuiToolBar
{
public:

	void SelectItem(wxAuiToolBarItem* item)
	{
		Realize();

		// Simulate click event so tool gets properly selected.
		wxRect itemRect = item->GetSizerItem()->GetRect();
		
		wxMouseEvent e;
		e.SetPosition(itemRect.GetPosition());

		wxAuiToolBar::OnLeftDown(e);
		wxAuiToolBar::OnLeftUp(e);
	}
};

void SelectionPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbar = editor->getToolbar();

	if(toolbar)
	{
		addTool( toolbar->AddSeparator() );

		wxBitmap iconSelect = wxMEMORY_BITMAP(cursor);
		buttonSelect = toolbar->AddTool( SelectionTool::Select, "Select",
			iconSelect, "Selects the selection tool", wxITEM_RADIO );
		addTool(buttonSelect, true);

		#pragma TODO(Remove the hack to select the select tool automatically)
		((ToolbarHack*) toolbar)->SelectItem(buttonSelect);
	}

	selections = nullptr;
	buttonSelect = nullptr;
	additiveMode = false;
	handlingEvents = false;
}

//-----------------------------------//

void SelectionPlugin::onPluginDisable()
{
	#pragma TODO("Check there are no active selections")
	//assert( !selections );
}

//-----------------------------------//

void SelectionPlugin::onToolSelect( int id )
{
}

//-----------------------------------//

void SelectionPlugin::onToolNone()
{
	// When there is no tool selected, select the 'Select' tool.
	ToolExtension* selectTool = findToolById(SelectionTool::Select);
	GetEditor().getEventManager()->setCurrentTool(selectTool);
}

//-----------------------------------//

void SelectionPlugin::onUndoOperation( const UndoOperationPtr& operation )
{
	assert( operation != nullptr );

	Class* klass = operation->getType();
	
	if( !ClassInherits(klass, ReflectionGetType(SelectionOperation)) )
		return;

	SelectionOperation* selection = RefCast<SelectionOperation>(operation).get();
	selection->redo();
}

//-----------------------------------//

void SelectionPlugin::onDocumentSelect( Document& document )
{
	DocumentContextMap& documentContext = document.documentContext;

	if( documentContext.find(this) == documentContext.end() )
	{
		documentContext[this] = AllocateHeap(SelectionManager);
	}

	selections = (SelectionManager*) documentContext[this].get();
}

//-----------------------------------//

void SelectionPlugin::onDocumentUnselect( Document& document )
{
	DocumentContextMap& documentContext = document.documentContext;

	auto it = documentContext.find(this);

	if( it == documentContext.end() )
		return;

	if( it->second.get() == selections )
	{
		// Clean up the current selection manager.
		selections = nullptr;
	}
}

//-----------------------------------//

void SelectionPlugin::onSceneLoad( const ScenePtr& scene )
{
	scene->entities.onEntityRemoved.Connect(this, &SelectionPlugin::onEntityRemoved);
}

//-----------------------------------//

void SelectionPlugin::onSceneUnload( const ScenePtr& scene )
{
	if( handlingEvents ) return;
	handlingEvents = true;

	scene->entities.onEntityRemoved.Disconnect(this, &SelectionPlugin::onEntityRemoved);

	if( selections )
		selections->removeCurrent();

	handlingEvents = false;
}

//-----------------------------------//

void SelectionPlugin::onEntityRemoved(const EntityPtr& entity)
{
	onEntityUnselect(entity);
}

//-----------------------------------//

void SelectionPlugin::onEntityUnselect(const EntityPtr& entity)
{
	if( handlingEvents ) return;
	handlingEvents = true;

	SelectionData data;
	data.mode = SelectionMode::Entity;
	data.entity = entity;

	selections->removeSelection(data);

	handlingEvents = false;
}

//-----------------------------------//

void SelectionPlugin::onKeyPress(const KeyEvent& event)
{
	EventManager* events = editor->getEventManager();
	
	bool isSelection = events->getCurrentToolId() == (int) SelectionTool::Select;
	if( !isSelection ) return;

	if( event.ctrlPressed )
	{
		SceneDocument* document = (SceneDocument*) editor->getDocument();
		document->getWindow()->SetCursor( wxCursor("cursorArrowPlus") );
		
		additiveMode = true;
	}
}

//-----------------------------------//

void SelectionPlugin::onKeyRelease(const KeyEvent& event)
{
	EventManager* events = editor->getEventManager();
	
	if( !event.ctrlPressed && additiveMode )
	{
		SceneDocument* document = (SceneDocument*) editor->getDocument();
		document->getWindow()->SetCursor( wxNullCursor );

		additiveMode = false;
	}
}

//-----------------------------------//

void SelectionPlugin::onMouseButtonPress( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Left )
		return;

	dragOrigin = Vector2(event.x, event.y );

	Document* document = editor->getDocument();
	if( !document ) return;
	
	SceneDocument* sceneDocument = (SceneDocument*) document;
	RenderWindow* window = sceneDocument->getRenderWindow();
	window->setCursorCapture(true);
}

//-----------------------------------//

void SelectionPlugin::onMouseButtonRelease( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Left )
		return;

	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	sceneDocument->getRenderWindow()->setCursorCapture(false);

	editor->getDocument()->getWindow()->flagRedraw();

	SelectionOperation* selection = nullptr;

	if(selections->dragRectangle)
		selection = processDragSelection(event);
	else
		selection = processSelection(event);

	if( !selection ) return;

	const SelectionCollection& selected = selections->getSelections();

	// Prevent duplication of selection events.
	if( selected.isSame(selection->selections) ) 
	{
		LogDebug("Ignoring duplicated selection");
		Deallocate(selection);
		return;
	}

	selection->redo();

	UndoManager* undoManager = sceneDocument->getUndoManager();
	undoManager->registerOperation(selection);
}

//-----------------------------------//

void SelectionPlugin::onMouseDrag( const MouseDragEvent& event )
{
	EventManager* events = editor->getEventManager();

	if( events->getCurrentToolId() != SelectionTool::Select )
		return;

	if( !event.info->leftButton )
		return;

	if( !selections->dragRectangle )
	{
		selections->dragRectangle = createRectangle();

		Document* document = editor->getDocument();
		if( !document ) return;
	
		SceneDocument* sceneDocument = (SceneDocument*) document;
		sceneDocument->editorScene->entities.add(selections->dragRectangle);
	}
	
	updateRectangle( event, selections->dragRectangle.get() );
}

//-----------------------------------//

EntityPtr SelectionPlugin::createRectangle()
{
	Overlay* overlay = AllocateThis(Overlay);
	overlay->setPositionMode( PositionMode::Absolute );
	overlay->setOpacity(0.3f);
	overlay->setBorderWidth(1);
	overlay->setBorderColor( Color::White );
	overlay->setBackgroundColor( Color::White );

	Entity* dragRectangle = EntityCreate( AllocatorGetHeap() );
	dragRectangle->addTransform();
	dragRectangle->addComponent(overlay);

	return dragRectangle;
}

//-----------------------------------//

void SelectionPlugin::updateRectangle( const MouseDragEvent& event, Entity* dragRectangle )
{
	Vector2 dragPoint = Vector2(event.x, event.y);
	
	Vector2 dragMin;
	dragMin.x = std::min(dragOrigin.x, dragPoint.x);
	dragMin.y = std::min(dragOrigin.y, dragPoint.y);

	Vector2 dragMax;
	dragMax.x = std::max(dragOrigin.x, dragPoint.x);
	dragMax.y = std::max(dragOrigin.y, dragPoint.y);

	Overlay* overlay = dragRectangle->getComponent<Overlay>().get();
	overlay->setPositionMode(PositionMode::Absolute);
	overlay->setOffset(dragMin);
	overlay->setSize( dragMax - dragMin );

	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::createDeselection()
{
	// If there is a current selection, and the user pressed the mouse,
	// then we need to unselect everything that is currently selected.

	SelectionOperation* selection = CreateSelectionOperation("Deselection");
	selection->setPreviousSelections( selections->getSelections() );

	return selection;
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::processDragSelection(const MouseButtonEvent& event)
{
	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	Scene* scene = sceneDocument->scene.get();
	
	RenderView* view = sceneDocument->sceneWindow->getView();
	Camera* camera = sceneDocument->sceneWindow->getCamera().get();

	Overlay* overlay = selections->dragRectangle->getComponent<Overlay>().get();
	const Vector3& pos = overlay->getOffset();
	const Vector3& size = overlay->getSize();

	Frustum pickVolume = camera->getVolume(pos.x, pos.x+size.x, pos.y, pos.y+size.y);

	RayQueryList list;
	scene->doRayVolumeQuery(pickVolume, list);

	sceneDocument->editorScene->entities.remove(selections->dragRectangle);
	selections->dragRectangle.reset();

	if( list.empty() )
	{
		SelectionOperation* selection = createDeselection();
		return selection;
	}

#if 0
	// If we are in additive mode, don't create a new selection.
	if( selected && additiveMode )
		selection = selected;
#endif

	// If there is no current selection, create a new one.
	SelectionOperation* selection = CreateSelectionOperation("Drag Selection");
	selection->setPreviousSelections( selections->getSelections() );

	SelectionCollection& selections = selection->selections;

	for( size_t i = 0; i < list.size(); i++ )
	{
		// Add the picked selection to the selection collection.
		selections.addEntity( list[i].entity );
	}

#if 0
	if( selected && !additiveMode )
		selection->previous = selected->selections;
#endif

	return selection;
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::processSelection(const MouseButtonEvent& event)
{
	SelectionOperation* selection = nullptr;

	EntityPtr entity;
	
	if( !getPickEntity(event.x, event.y, entity) )
	{
		selection = createDeselection();
		return selection;
	}
	
	selection = CreateSelectionOperation("Selection");
	selection->selections.addEntity(entity);
	selection->setPreviousSelections( selections->getSelections() );

	return selection;
}

//-----------------------------------//

bool SelectionPlugin::getPickEntity(int x, int y, EntityPtr& entity)
{
	Document* document = editor->getDocument();
	if( !document ) return false;

	SceneDocument* sceneDocument = (SceneDocument*) document;
	sceneDocument->editorScene->entities.remove(selections->dragRectangle);

	RenderView* view = sceneDocument->sceneWindow->getView();
	Camera* camera = sceneDocument->sceneWindow->getCamera().get();
	Scene* scene = sceneDocument->scene.get();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay(x, y, &outFar);

#if 0 // Enable this to draw debugging lines
	camera->drawer.drawRay(pickRay, (outFar - pickRay.origin).length());
#endif

	// Perform ray casting to find the entities.
	// The results are sorted by proximity to the camera.
	RayQueryList list;
	
	if( !scene->doRayBoxQuery(pickRay, list) )
		return false;

	float minDistance = LimitsFloatMaximum;

	bool found = false;

	for( size_t i = 0; i < list.size(); i++ )
	{
		const RayQueryResult& query = list[i];
		
		// Skip entities that we are inside of.
		//if( query.distance == 0 ) continue;

		RayTriangleQueryResult res;

		if( !scene->doRayTriangleQuery(pickRay, res, query.entity) )
			continue;

		if( res.distance < minDistance )
		{
			found = true;
			minDistance = res.distance;
			entity = res.entity;
		}
	}
	
	return found;
}

//-----------------------------------//

SelectionManager* SelectionPlugin::getSelectionManager() const
{
	return selections;
}

//-----------------------------------//

NAMESPACE_EDITOR_END
