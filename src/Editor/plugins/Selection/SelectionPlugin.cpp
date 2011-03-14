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

#include "Events.h"
#include "UndoManager.h"

#include "Editor.h"
#include "EditorIcons.h"

#include "../Scene/SceneDocument.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CHILD_CLASS(SelectionPlugin, Plugin)
REFLECT_END()

//-----------------------------------//

namespace SelectionTool
{
	enum Enum
	{
		Select = wxID_HIGHEST + 42
	};
}

//-----------------------------------//

SelectionPlugin::SelectionPlugin()
	: Plugin()
	, selections(nullptr)
	, buttonSelect(nullptr)
	, dragRectangle(nullptr)
{ }

//-----------------------------------//

PluginMetadata SelectionPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Selection";
	metadata.description = "Provides selection services";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void SelectionPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbar = editor->getToolbar();

	if(toolbar)
	{
		addTool( toolbar->AddSeparator() );

		wxBitmap iconSelect = wxMEMORY_BITMAP(cursor);
		buttonSelect = toolbar->AddTool( SelectionTool::Select, "Select",
			iconSelect, "Selects the Entity Selection tool", wxITEM_RADIO );
		addTool(buttonSelect, true);
	}

	#pragma TODO("Initialize plugins events properly")
	
	Events* events = editor->getEventManager();
	events->setCurrentPlugin(this);
	events->setCurrentTool((int) SelectionTool::Select);
	events->addEventListener(this);

	selections = new SelectionManager;
}

//-----------------------------------//

void SelectionPlugin::onPluginDisable()
{
	delete selections;
	selections = nullptr;
}

//-----------------------------------//

void SelectionPlugin::onSceneLoad( const ScenePtr& scene )
{
	SelectionOperation* selection = selections->getSelection();

	if( selection )
		selection->unselectAll();

	scene->onEntityRemoved.Connect(this, &SelectionPlugin::onEntityRemoved);
}

//-----------------------------------//

void SelectionPlugin::onEntityRemoved(const EntityPtr& entity)
{
	SelectionOperation* op = selections->getSelection();

	if(op && op->isSelection(entity))
	{
		Events* events = editor->getEventManager();
		events->onEntityUnselect(entity);
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

	Document* document = editor->getDocument();
	if( !document ) return;
	SceneDocument* sceneDocument = (SceneDocument*) document;
	
	RenderWindow* window = sceneDocument->getRenderWindow();
	window->setCursorCapture(false);

	editor->redrawView();

	SelectionOperation* selection = nullptr;

	if(dragRectangle)
		selection = processDragSelection(event);
	else
		selection = processSelection(event);

	if( !selection )
		return;

	SelectionOperation* selected = selections->getSelection();

	// Prevent duplication of selection events.
	if( selected && (!selected->lastUndone)
		&& (selected->selections == selection->selections))
	{
		delete selection;
		return;
	}

	if( selected )
		selected->unselectAll();

	UndoManager* undoManager = document->getUndoManager();
	undoManager->registerOperation(selection);

	selection->redo();
}

//-----------------------------------//

void SelectionPlugin::onMouseDrag( const MouseDragEvent& event )
{
	Events* events = editor->getEventManager();

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

	dragRectangle.reset( new Entity() );
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

	if( !selected )
		return nullptr;

	// If there is a current selection, and the user pressed the mouse,
	// then we need to unselect everything that is currently selected.

	SelectionOperation* selection = selections->createOperation();
	selection->description = "Deselection";
	selection->mode = SelectionMode::None;
	selection->previous = selected->selections;

	return selection;
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::processDragSelection(const MouseButtonEvent& event)
{
	Document* document = editor->getDocument();
	if( !document ) return nullptr;
	SceneDocument* sceneDocument = (SceneDocument*) document;

	const ScenePtr& scene = sceneDocument->scene;
	RenderView* view = sceneDocument->viewFrame->getView();
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

	selection = selections->createOperation();
	selection->description = "Drag Selection";

	if( selected )
		selection->previous = selected->selections;

	for(uint i = 0; i < list.size(); i++ )
	{
		selection->addEntity( list[i].entity );
	}

	return selection;
}

//-----------------------------------//

SelectionOperation* SelectionPlugin::processSelection(const MouseButtonEvent& event)
{
	SelectionOperation* selection = nullptr;
	SelectionOperation* selected = selections->getSelection();

	EntityPtr entity;
	
	if( !getPickEntity(event.x, event.y, entity) )
	{
		selection = createDeselection();
		return selection;
	}
	
	selection = selections->createOperation();
	selection->description = "Selection";

	if( selected && !selected->lastUndone )
		selection->previous = selected->selections;
	else if( selected )
		selection->previous = selected->previous;

	selection->addEntity(entity);

	return selection;
}

//-----------------------------------//

bool SelectionPlugin::getPickEntity(int x, int y, EntityPtr& entity)
{
	Document* document = editor->getDocument();
	if( !document ) return false;
	SceneDocument* sceneDocument = (SceneDocument*) document;
	
	sceneDocument->editorScene->remove(dragRectangle);

	RenderView* view = sceneDocument->viewFrame->getView();
	const CameraPtr& camera = view->getCamera();
	const ScenePtr& scene = sceneDocument->scene;

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( x, y, &outFar );

#if 0 // Enable this to draw debugging lines
	const EntityPtr& line = buildRay( pickRay, outFar );
	editor->getEditorScene()->add( line );
#endif

	// Perform ray casting to find the entities.
	// The results are sorted by proximity to the camera.
	RayQueryList list;
	
	if( !scene->doRayBoxQuery(pickRay, list) )
		return false;

	float minDistance = LimitsFloatMaximum;

	bool found = false;

	for( uint i = 0; i < list.size(); i++ )
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

} } // end namespaces
