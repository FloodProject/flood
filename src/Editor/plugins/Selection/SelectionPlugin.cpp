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

#include "EventManager.h"
#include "UndoManager.h"

#include "Editor.h"
#include "EditorIcons.h"

#include "../Scene/SceneDocument.h"

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
	: selections(nullptr)
	, buttonSelect(nullptr)
	, dragRectangle(nullptr)
	, additiveMode(false)
{

}

//-----------------------------------//

PluginMetadata SelectionPlugin::getMetadata()
{
	PluginMetadata metadata;
	
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

		((ToolbarHack*) toolbar)->SelectItem(buttonSelect);
	}

	EventManager* events = editor->getEventManager();
	//events->addEventListener(this);
}

//-----------------------------------//

void SelectionPlugin::onPluginDisable()
{
	assert( !selections );
}

//-----------------------------------//

void SelectionPlugin::onToolSelect( int id )
{
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

void SelectionPlugin::onSceneLoad( const ScenePtr& scene )
{
	assert( !selections );

	scene->onEntityRemoved.Connect(this, &SelectionPlugin::onEntityRemoved);
	selections = AllocateThis(SelectionManager);
}

//-----------------------------------//

void SelectionPlugin::onSceneUnload( const ScenePtr& scene )
{
	assert( selections );

	scene->onEntityRemoved.Disconnect(this, &SelectionPlugin::onEntityRemoved);

	SelectionOperation* selection = selections->getSelection();

	if(selection)
	{
		selection->unselectAll();
		selections->setSelection(nullptr);
	}

	Deallocate(selections);
}

//-----------------------------------//

void SelectionPlugin::onEntityRemoved(const EntityPtr& entity)
{
	SelectionOperation* selection = selections->getSelection();
	bool isSelected = selection && selection->isSelection(entity);

	if(isSelected)
	{
		selection->unselectAll();
		selections->setSelection(nullptr);
	}
}

//-----------------------------------//

void SelectionPlugin::onEntityUnselect(const EntityPtr& entity)
{
#if 0
	SelectionOperation* op = selections->getSelection();
	bool isSelected = op && op->isSelection(entity);
	
	if(isSelected)
#endif
}

//-----------------------------------//

void SelectionPlugin::onKeyPress(const KeyEvent& event)
{
	EventManager* events = editor->getEventManager();
	
	bool isSelection = events->getCurrentTool() == (int) SelectionTool::Select;
	if( !isSelection ) return;

	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	RenderControl* control = sceneDocument->getRenderControl();

	if( event.ctrlPressed )
	{
		control->SetCursor( wxCursor("cursorArrowPlus") );
		additiveMode = true;
	}
}

//-----------------------------------//

void SelectionPlugin::onKeyRelease(const KeyEvent& event)
{
	EventManager* events = editor->getEventManager();
	
	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	RenderControl* control = sceneDocument->getRenderControl();

	if( !event.ctrlPressed && additiveMode )
	{
		control->SetCursor( wxNullCursor );
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

static bool isSameSelection(SelectionOperation* oldSelection, SelectionOperation* newSelection)
{
	bool isSameMode = (oldSelection->mode == newSelection->mode);
	bool isSameObjects = (oldSelection->selections == newSelection->selections);
	return isSameMode && (!oldSelection->lastUndone) && isSameObjects;
}

void SelectionPlugin::onMouseButtonRelease( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Left )
		return;

	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	sceneDocument->getRenderWindow()->setCursorCapture(false);

	editor->redrawView();

	SelectionOperation* selection = nullptr;

	if(dragRectangle)
		selection = processDragSelection(event);
	else
		selection = processSelection(event);

	if( !selection ) return;

	SelectionOperation* selected = selections->getSelection();

	// Prevent duplication of selection events.
	if(selected && isSameSelection(selected, selection)) 
	{
		LogDebug("Ignoring duplicated selection");
		Deallocate(selection);
		return;
	}

	if( selected )
		selected->unselectAll();
		
	selection->redo();

	UndoManager* undoManager = sceneDocument->getUndoManager();
	undoManager->registerOperation(selection);
}

//-----------------------------------//

void SelectionPlugin::onMouseDrag( const MouseDragEvent& event )
{
	EventManager* events = editor->getEventManager();

	if( events->getCurrentTool() != SelectionTool::Select )
		return;

	if( !event.info->leftButton )
		return;

	if( !dragRectangle ) createRectangle();
	
	updateRectangle( event );
}

//-----------------------------------//

void SelectionPlugin::createRectangle()
{
	OverlayPtr overlay( new Overlay() );
	overlay->setPositionMode( PositionMode::Absolute );
	overlay->setOpacity(0.5f);
	overlay->setBorderWidth(1);
	overlay->setBorderColor( Color::White );
	overlay->setBackgroundColor( Color::White );

	dragRectangle.reset( EntityCreate( AllocatorGetHeap() ) );
	dragRectangle->addTransform();
	dragRectangle->addComponent(overlay);

	Document* document = editor->getDocument();
	if( !document ) return;
	SceneDocument* sceneDocument = (SceneDocument*) document;
	sceneDocument->editorScene->add(dragRectangle);
}

//-----------------------------------//

void SelectionPlugin::updateRectangle( const MouseDragEvent& event )
{
	Vector2 dragPoint = Vector2(event.x, event.y);
	
	Vector2 dragMin;
	dragMin.x = std::min(dragOrigin.x, dragPoint.x);
	dragMin.y = std::min(dragOrigin.y, dragPoint.y);

	Vector2 dragMax;
	dragMax.x = std::max(dragOrigin.x, dragPoint.x);
	dragMax.y = std::max(dragOrigin.y, dragPoint.y);

	OverlayPtr overlay = dragRectangle->getComponent<Overlay>();
	overlay->setPosition(dragMin);
	overlay->setSize( dragMax - dragMin );

	editor->redrawView();
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::createDeselection()
{
	SelectionOperation* selected = selections->getSelection();
	if( !selected ) return nullptr;

	// If there is a current selection, and the user pressed the mouse,
	// then we need to unselect everything that is currently selected.

	SelectionOperation* selection = selections->createOperation( selections->getSelectionMode() );
	selection->description = "Deselection";
	selection->mode = SelectionMode::None;
	
	// In the case of an undone deselection, the selection is in the previous selections.
	if( selected->selections.empty() )
		selection->previous = selected->previous;
	else
		selection->previous = selected->selections;

	return selection;
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::processDragSelection(const MouseButtonEvent& event)
{
	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	const ScenePtr& scene = sceneDocument->scene;
	RenderView* view = sceneDocument->viewframe->getView();
	const CameraPtr& camera = view->getCamera();

	OverlayPtr overlay = dragRectangle->getComponent<Overlay>();
	const Vector2& pos = overlay->getPosition();
	const Vector2& size = overlay->getSize();

	Frustum pickVolume = camera->getVolume(pos.x, pos.x+size.x, pos.y, pos.y+size.y);

	RayQueryList list;
	scene->doRayVolumeQuery(pickVolume, list);

	SelectionOperation* selection = nullptr;
	SelectionOperation* selected = selections->getSelection();

	sceneDocument->editorScene->remove(dragRectangle);
	dragRectangle.reset();

	if( list.empty() )
	{
		selection = createDeselection();
		return selection;
	}

	// If we are in additive mode, don't create a new selection.
	if( selected && additiveMode )
		selection = selected;

	// If there is no current selection, create a new one.
	if( !selection )
	{
		selection = selections->createOperation( selections->getSelectionMode() );
		selection->description = "Drag Selection";
	}

	if( selected && !additiveMode )
		selection->previous = selected->selections;

	for( size_t i = 0; i < list.size(); i++ )
	{
		selection->addEntity( list[i].entity );
	}

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
	
	selection = selections->createOperation( selections->getSelectionMode() );
	selection->description = "Selection";
	selection->addEntity(entity);

	SelectionOperation* selected = selections->getSelection();
	selection->setPreviousSelections(selected);

	return selection;
}

//-----------------------------------//

bool SelectionPlugin::getPickEntity(int x, int y, EntityPtr& entity)
{
	Document* document = editor->getDocument();
	if( !document ) return false;
	SceneDocument* sceneDocument = (SceneDocument*) document;
	
	sceneDocument->editorScene->remove(dragRectangle);

	RenderView* view = sceneDocument->viewframe->getView();
	const CameraPtr& camera = view->getCamera();
	const ScenePtr& scene = sceneDocument->scene;

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
