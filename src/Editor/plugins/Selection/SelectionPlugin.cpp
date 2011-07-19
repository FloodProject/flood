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

REFLECT_CHILD_CLASS(SelectionPlugin, EditorPlugin)
REFLECT_CLASS_END()

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
	: selections(nullptr)
	, buttonSelect(nullptr)
	, dragRectangle(nullptr)
	, additiveMode(false)
{ }

//-----------------------------------//

PluginMetadata SelectionPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = PLUGIN_SELECTION;
	metadata.description = "Provides selection services.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 15;

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
			iconSelect, "Selects the selection tool", wxITEM_RADIO );
		addTool(buttonSelect, true);
	}

	Events* events = editor->getEventManager();
	events->addEventListener(this);

	selections = new SelectionManager();
}

//-----------------------------------//

void SelectionPlugin::onDocumentCreate( Document& document )
{
	wxAuiToolBar* tb = document.createContextToolbar();
	tb->SetMargins(0, 0, 0, 0);
	//tb->SetSize(wxSize(-1, 20));
	
#if 0	
	tb->AddLabel(wxID_ANY, "Selection");
	tb->AddTool(wxID_ANY, wxMEMORY_BITMAP(page_code), wxMEMORY_BITMAP(page_code));
#endif

	wxButton* button = new wxButton(tb, wxID_ANY, "Group", wxDefaultPosition, wxSize(-1, 20), wxBU_EXACTFIT);
	tb->AddControl(button);

#if 0	
	wxComboBox* comboBox = new wxComboBox(tb, wxID_ANY);
	comboBox->SetSize(-1, 16);
	comboBox->Append("Object");
	comboBox->Append("Vertex");
	comboBox->Append("Face");
	tb->AddControl(comboBox, "Mode");
#endif

	PluginTool* selectTool = findTool(buttonSelect);
	selectTool->setToolbar(tb);

	Events* events = GetEditor().getEventManager();
	events->setTool(this, selectTool);
}

//-----------------------------------//

void SelectionPlugin::onPluginDisable()
{
	delete selections;
	selections = nullptr;
}

//-----------------------------------//

void SelectionPlugin::onToolSelect( int id )
{
	SceneDocument* document = (SceneDocument*) editor->getDocument();
	Viewframe* viewframe = document->getViewframe();
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

void SelectionPlugin::onKeyPress(const KeyEvent& event)
{
	Events* events = editor->getEventManager();
	
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
	Events* events = editor->getEventManager();
	
	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	RenderControl* control = sceneDocument->getRenderControl();

	if( !event.ctrlPressed && additiveMode )
	{
		// http://trac.wxwidgets.org/ticket/12961
		#pragma TODO("Change back to wxNullCursor once wxWidgets bug has been fixed")
		
		control->SetCursor( *wxSTANDARD_CURSOR );
		control->Update();
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

	editor->redrawView();

	SelectionOperation* selection = nullptr;

	if(dragRectangle)
		selection = processDragSelection(event);
	else
		selection = processSelection(event);

	if( !selection ) return;

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

	UndoManager* undoManager = sceneDocument->getUndoManager();
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

	SelectionOperation* selection = selections->createOperation();
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
		selection = selections->createOperation();
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

	RenderView* view = sceneDocument->viewframe->getView();
	const CameraPtr& camera = view->getCamera();
	const ScenePtr& scene = sceneDocument->scene;

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( x, y, &outFar );

#if 0 // Enable this to draw debugging lines
	const EntityPtr& line = buildRay( pickRay, outFar );
	sceneDocument->editorScene->add( line );
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

} } // end namespaces
