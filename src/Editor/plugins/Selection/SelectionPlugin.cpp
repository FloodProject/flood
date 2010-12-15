/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "SelectionPlugin.h"
#include "SelectionManager.h"

#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"
#include "UndoManager.h"
#include "Viewframe.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace SelectionTool
{
	enum Enum
	{
		Select = wxID_HIGHEST + 42
	};
}

//-----------------------------------//

SelectionPlugin::SelectionPlugin( EditorFrame* frame )
	: Plugin(frame)
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
	wxToolBar* toolbar = editor->getToolbar();

	wxBitmap iconSelect = wxMEMORY_BITMAP(cursor);
	buttonSelect = toolbar->AddTool( SelectionTool::Select, "Select",
		iconSelect, "Selects the Entity Selection tool", wxITEM_RADIO );
	addTool(buttonSelect);

	#pragma TODO("Initialize plugins events properly")
	Events* events = editor->getEventManager();
	events->currentPlugin = this;

	selections = new SelectionManager(editor);
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
}

//-----------------------------------//

void SelectionPlugin::onMouseButtonPress( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Left )
		return;

	dragOrigin = Vector2i(event.x, event.y );
}

//-----------------------------------//

void SelectionPlugin::onMouseButtonRelease( const MouseButtonEvent& event )
{
	if( event.button != MouseButton::Left )
		return;

	SelectionOperation* selection = nullptr;
	SelectionOperation* selected = selections->getSelection();

	if(dragRectangle)
		selection = processDragSelection(event);
	else
		selection = processSelection(event);

	if( !selection )
		return;

	// Prevent duplication of selection events.
	if( selected && (!selected->lastUndone)
		&& (selected->selections == selection->selections))
	{
		delete selection;
		return;
	}

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation(selection);

	if( selected )
		selected->unselectAll();

	selection->redo();
}

//-----------------------------------//

void SelectionPlugin::onMouseDrag( const MouseDragEvent& event )
{
	if( !event.info->leftButton )
		return;

	if( !dragRectangle )
	{
		createRectangle();
	}

	updateRectangle( event );
}

//-----------------------------------//

void SelectionPlugin::createRectangle()
{
	OverlayPtr overlay( new Overlay() );
	overlay->setPositionMode( PositionMode::Absolute );
	//overlay->setOpacity();

	MaterialPtr material = overlay->getRenderables()[0]->getMaterial();
	material->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha);

	dragRectangle.reset( new Entity() );
	dragRectangle->addTransform();
	dragRectangle->addComponent(overlay);
	editor->getEditorScene()->add(dragRectangle);
}

//-----------------------------------//

void SelectionPlugin::updateRectangle( const MouseDragEvent& event )
{
	Vector2i dragPoint = Vector2i(event.x, event.y);
	
	Vector2i dragMin;
	dragMin.x = std::min(dragOrigin.x, dragPoint.x);
	dragMin.y = std::min(dragOrigin.y, dragPoint.y);

	Vector2i dragMax;
	dragMax.x = std::max(dragOrigin.x, dragPoint.x);
	dragMax.y = std::max(dragOrigin.y, dragPoint.y);

	OverlayPtr overlay = dragRectangle->getComponent<Overlay>();
	overlay->setPosition(dragMin);
	overlay->setSize( dragMax - dragMin );
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
	const ScenePtr& scene = editor->getEngine()->getSceneManager();
	RenderView* view = editor->getMainViewframe()->getView();
	const CameraPtr& camera = view->getCamera();

	OverlayPtr overlay = dragRectangle->getComponent<Overlay>();
	const Vector2i& pos = overlay->getPosition();
	const Vector2i& size = overlay->getSize();

	Frustum pickVolume = camera->getVolume(pos.x, pos.x+size.x, pos.y, pos.y+size.y);

	RayQueryList list;
	scene->doRayVolumeQuery(pickVolume, list);

	SelectionOperation* selection = nullptr;
	SelectionOperation* selected = selections->getSelection();

	editor->getEditorScene()->remove(dragRectangle);
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

	if( selected )
		selection->previous = selected->selections;

	selection->addEntity(entity);

	return selection;
}

//-----------------------------------//

bool SelectionPlugin::getPickEntity(int x, int y, EntityPtr& entity)
{
	Viewframe* viewframe = editor->getMainViewframe();
	RenderView* view = viewframe->getView();
	
	const CameraPtr& camera = view->getCamera();
	const ScenePtr& scene = engine->getSceneManager();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( x, y, &outFar );

#if 0 // Enable this to draw debugging lines
	const EntityPtr& line = buildRay( pickRay, outFar );
	editor->getEditorScene()->add( line );
#endif

	// Perform ray casting to find the nodes.
	RayQueryResult res;
	if( !scene->doRayBoxQuery(pickRay, res) )
		return false;

	entity = res.entity;
	return true;
}

//-----------------------------------//

SelectionManager* SelectionPlugin::getSelectionManager() const
{
	return selections;
}

//-----------------------------------//

} } // end namespaces