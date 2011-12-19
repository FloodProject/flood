/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "GizmoPlugin.h"
#include "GizmoOperation.h"
#include "GizmoTranslate.h"
#include "GizmoRotate.h"
#include "GizmoScale.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "EventManager.h"
#include "UndoManager.h"
#include "DocumentWindow.h"
#include "Core/PluginManager.h"

#include "Plugins/Selection/SelectionPlugin.h"
#include "Plugins/Selection/SelectionManager.h"
#include "Plugins/Scene/SceneDocument.h"

#ifdef ENABLE_PLUGIN_GIZMO

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(GizmoPlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

GizmoPlugin::GizmoPlugin()
{
}

//-----------------------------------//

PluginMetadata GizmoPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Gizmos";
	metadata.description = "Provides translate, rotate and scale tools.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 280;

	return metadata;
}

//-----------------------------------//

SelectionManager* GizmoPlugin::getSelections()
{
	SelectionPlugin* sp = GetPlugin<SelectionPlugin>();
	return sp->getSelectionManager();
}

//-----------------------------------//

void GizmoPlugin::onPluginEnable()
{
	wxAuiToolBar* toolbarCtrl = editor->getToolbar();

	if(toolbarCtrl)
	{
		wxBitmap iconCamera = wxMEMORY_BITMAP(camera);
		buttonCamera = toolbarCtrl->AddTool( GizmoTool::Camera, "Camera",
			iconCamera, "Selects the Camera View tool", wxITEM_RADIO );
		addTool(buttonCamera, true);

		wxBitmap iconTranslate = wxMEMORY_BITMAP(move);
		buttonTranslate = toolbarCtrl->AddTool( GizmoTool::Translate, "Move",
			iconTranslate, "Selects the Move tool", wxITEM_RADIO );
		addTool(buttonTranslate, true);

		wxBitmap iconRotate = wxMEMORY_BITMAP(rotate2);
		buttonRotate = toolbarCtrl->AddTool( GizmoTool::Rotate, "Rotate",
			iconRotate, "Selects the Rotate tool", wxITEM_RADIO );
		addTool(buttonRotate, true);

		wxBitmap iconScale = wxMEMORY_BITMAP(scale);
		buttonScale = toolbarCtrl->AddTool( GizmoTool::Scale, "Scale",
			iconScale, "Selects the Scale tool", wxITEM_RADIO );
		addTool(buttonScale, true);
	}

	reset();
}

//-----------------------------------//

void GizmoPlugin::onPluginDisable()
{
	reset();
}

//-----------------------------------//

void GizmoPlugin::reset()
{
	gizmo = nullptr;
	gizmos.clear();

	isGizmoPicked = false;
	op = nullptr;

	axis = GizmoAxis::None;
}

//-----------------------------------//

void GizmoPlugin::onToolSelect( int id )
{
	SelectionManager* selectionManager = getSelections();
	if( !selectionManager ) return;

	// Simulate an object selection event when this tool is selected.
	const SelectionCollection& selections = selectionManager->getSelections();
	onSelection(selections);
}

//-----------------------------------//

void GizmoPlugin::onToolUnselect( int id )
{
	SelectionManager* selectionManager = getSelections();
	if( !selectionManager ) return;

	const SelectionCollection& selections = selectionManager->getSelections();
	onDeselection(selections);
}

//-----------------------------------//

void GizmoPlugin::onSelection( const SelectionCollection& collection )
{
	if( collection.isEmpty() ) return;

	const SelectionsVector& selections = collection.selections;

	for( size_t i = 0; i < selections.size(); i++ )
	{
		const SelectionData& data = selections[i];

		switch(data.mode)
		{
		case SelectionMode::Entity:
		{
			onEntityUnselect( data.entity );
			onEntitySelect( data.entity );
			break;
		}
		default:
			LogDebug("Unsupported selection mode");
			continue;
		}
	}
}

//-----------------------------------//

void GizmoPlugin::onDeselection( const SelectionCollection& collection )
{
	if( collection.isEmpty() ) return;

	const SelectionsVector& selections = collection.selections;

	for( size_t i = 0; i < selections.size(); i++ )
	{
		const SelectionData& data = selections[i];

		switch(data.mode)
		{
		case SelectionMode::Entity:
		{
			onEntityUnselect( data.entity );
			break;
		}
		default:
			LogDebug("Unsupported selection mode");
			continue;
		}
	}
}

//-----------------------------------//

void GizmoPlugin::onSceneUnload(const ScenePtr&)
{
	reset();
}

//-----------------------------------//

void GizmoPlugin::onEntitySelect( const EntityPtr& entity )
{
	if( IsToolSelected(GizmoTool::Camera) )
		return;
	
	bool gizmoExists = gizmos.find(entity) != gizmos.end();
	if( gizmoExists ) return;

	createGizmo( entity );

	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

void GizmoPlugin::onEntityUnselect( const EntityPtr& entity )
{
	removeGizmo( entity );
	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

Plane GizmoPlugin::getGizmoPickPlane(const Ray& ray)
{
	const EntityPtr entity = op->weakEntity;
	const TransformPtr& transform = entity->getTransform();
	
	Vector3 gizmoPosition = transform->getPosition();
	Vector3 gizmoAxis = gizmo->getAxisVector(op->axis);
	
	Plane planeX( Vector3::UnitX, gizmoPosition );
	Plane planeY( Vector3::UnitY, gizmoPosition );
	Plane planeZ( Vector3::UnitZ, gizmoPosition );

	float lengthX = planeX.project(ray.direction).lengthSquared();
	float lengthY = planeY.project(ray.direction).lengthSquared();
	float lengthZ = planeZ.project(ray.direction).lengthSquared();

	if( lengthX < lengthY && lengthX < lengthZ )
		return planeX;
	else if( lengthY < lengthX && lengthY < lengthZ )
		return planeY;
	else
		return planeZ;
}

//-----------------------------------//

bool GizmoPlugin::getGizmoPickPoint(int x, int y, Vector3& pickPoint)
{
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	Camera* camera = document->sceneWindow->getCamera().get();

	Ray ray = camera->getRay(x, y);

	float distance;
	
	if( !pickPlane.intersects(ray, distance) )
		return false;

	pickPoint = ray.getPoint(distance);
	
	return true;
}

//-----------------------------------//

void GizmoPlugin::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !gizmo ) return;

	if( IsToolSelected(GizmoTool::Camera) )
		return;

	// To check if the user picked a gizmo we use two hit test techniques.
	// The first technique is based on bounding volumes so it is not very
	// accurate. If it hits, a more accurate color picking will be done.

	if( pickBoundingTest(moveEvent) && pickImageTest(moveEvent, axis) )
	{
		if( !gizmo->isAnyAxisSelected() )
		{
			LogDebug("Setting size cursor");

			wxCursor cursor(wxCURSOR_SIZING);
			wxSetCursor(cursor);
		}

		gizmo->selectAxis(axis);

		if( !op )
			createOperation();
		else
			op->axis = axis;
	}
	else
	{
		//LogDebug("Setting null cursor");
		wxSetCursor(wxNullCursor);

		gizmo->deselectAxis();
		Deallocate(op);
	}

	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

void GizmoPlugin::onMouseDrag( const MouseDragEvent& dragEvent )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	if( !op ) return;

	if( !isGizmoPicked )
		return;

	Vector3 pickPoint;
	
	if( !getGizmoPickPoint(dragEvent.x, dragEvent.y, pickPoint) )
		return;

	Vector3 pickDelta = pickPoint - firstPickPoint;

	Vector3 pickAxis = gizmo->getAxisVector(op->axis);
	Vector3 pickOffset = pickAxis.project(pickDelta);

	const EntityPtr entityObject( op->weakEntity );
	const TransformPtr& transObject = entityObject->getTransform();
	
	if( IsToolSelected(GizmoTool::Translate) )
	{
		const Vector3& position = transObject->getPosition();
		transObject->setPosition( position + pickOffset*pickAxis );
	}

	firstPickPoint = pickPoint;

	op->scale = transObject->getScale();
	op->rotation = transObject->getRotation();
	op->translation = transObject->getPosition();

	editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( IsToolSelected(GizmoTool::Camera) ) return;

	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	sceneDocument->getRenderWindow()->setCursorCapture(true);

	if( mbe.button != MouseButton::Left )
		return;
	
	if( gizmo && gizmo->isAnyAxisSelected() )
	{
		isGizmoPicked = true;

		SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
		Camera* camera = document->sceneWindow->getCamera().get();
		
		Ray ray = camera->getRay(mbe.x, mbe.y);
		pickPlane = getGizmoPickPlane(ray);

		getGizmoPickPoint(mbe.x, mbe.y, firstPickPoint);
		return;
	}

	GetPlugin<SelectionPlugin>()->onMouseButtonPress(mbe);
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonRelease( const MouseButtonEvent& mbe )
{
	if( IsToolSelected(GizmoTool::Camera) ) return;

	SceneDocument* sceneDocument = (SceneDocument*) editor->getDocument();
	sceneDocument->getRenderWindow()->setCursorCapture(false);

	isGizmoPicked = false;

	if( !op ) goto select;

	UndoManager* undoManager = GetEditor().getDocument()->getUndoManager();
	undoManager->registerOperation( op );
	op = nullptr;
	
	if(gizmo)
	{
		gizmo->deselectAxis();
		return;
	}

select:

	GetPlugin<SelectionPlugin>()->onMouseButtonRelease(mbe);
}

//-----------------------------------//

void GizmoPlugin::createGizmo( const EntityPtr& entity )
{
	assert( entity != nullptr );
	assert( gizmos.find(entity) == gizmos.end() );

	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	Camera* camera = document->sceneWindow->getCamera().get();

	Gizmo* newGizmo = nullptr;

	// Create new Gizmo for the entity.
	if( IsToolSelected(GizmoTool::Translate) )
		newGizmo = new GizmoTranslate(entity, camera);

	else if( IsToolSelected(GizmoTool::Rotate) )
		newGizmo = new GizmoRotate(entity, camera);

	else if( IsToolSelected(GizmoTool::Scale) )
		newGizmo = new GizmoScale(entity, camera);

	else assert( 0 && "Unknown gizmo tool" );

	gizmo = newGizmo;
	gizmo->buildGeometry();

	// Add the gizmo to the scene.
	Entity* entityGizmo = EntityCreate( AllocatorGetHeap() );
	entityGizmo->setName("Gizmo");
	entityGizmo->addTransform();
	entityGizmo->addComponent(gizmo);
	document->editorScene->entities.add(entityGizmo);

	gizmo->updatePositionScale();
	
	// Associate the gizmo with the entity.
	gizmos[entity] = entityGizmo;
}

//-----------------------------------//

void GizmoPlugin::removeGizmo( const EntityPtr& entity )
{
	assert( entity != nullptr );

	// Find the existing gizmo associated with the entity.
	GizmoMap::iterator it = gizmos.find(entity);
	if( it == gizmos.end() ) return;

	// Find the entity of the gizmo.
	const EntityPtr& entityGizmo = gizmos[entity];
	
	// Remove the gizmo.
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	document->editorScene->entities.remove(entityGizmo);
	
	gizmos.erase(it);
	gizmo = nullptr;
}

//-----------------------------------//

bool GizmoPlugin::pickImageTest( const MouseMoveEvent& moveEvent, GizmoAxis::Enum& axis )
{
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	RenderView* view = document->sceneWindow->getView();
	Vector2i size = view->getSize();

	// We need to flip the Y-axis due to a mismatch between the 
	// OpenGL and wxWidgets coordinate-system origin conventions.
	int mouseX = moveEvent.x;
	int mouseY = size.y - moveEvent.y;

	RenderDevice* device = GetRenderDevice();
	Color pick = device->getRenderTarget()->getContext()->getPixel(mouseX, mouseY);

	axis = gizmo->getAxis(pick);
	return axis != GizmoAxis::None;
}

//-----------------------------------//

bool GizmoPlugin::pickBoundingTest( const MouseMoveEvent& me )
{
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	Camera* camera = document->getViewframe()->getCamera().get();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( me.x, me.y, &outFar );

	// Perform ray casting to find the entities.
	RayQueryResult res;
	
	if( !document->editorScene->doRayBoxQuery(pickRay, res) )
		return false;

	// Find out if we picked a gizmo...
	GizmoPtr gizmo = res.entity->getComponent<Gizmo>();
	
	return gizmo != nullptr;
}

//-----------------------------------//

void GizmoPlugin::createOperation()
{
	assert( op == nullptr );
	
	EntityPtr entityObject = gizmo->getEntityAttachment();
	TransformPtr transObject = entityObject->getTransform();

	op = AllocateThis(GizmoOperation);
	op->description = "Entity transform";

	op->tool = (GizmoTool::Enum) GetEditor().getEventManager()->getCurrentToolId();
	op->axis = axis;
	op->gizmo = gizmo;
	op->weakEntity = entityObject;

	op->prevTranslation = transObject->getPosition();
	op->prevScale = transObject->getScale();
	op->prevRotation = transObject->getRotation();
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif