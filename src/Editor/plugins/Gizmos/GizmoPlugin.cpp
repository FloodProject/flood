/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoPlugin.h"
#include "GizmoOperation.h"
#include "GizmoTranslate.h"
#include "GizmoRotate.h"
#include "GizmoScale.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"
#include "UndoManager.h"
#include "Viewframe.h"

namespace vapor { namespace editor {

//-----------------------------------//

GizmoPlugin::GizmoPlugin( EditorFrame* frame )
	: Plugin(frame)
	, editorScene(frame->getEditorScene())
	, op(nullptr)
{ }

//-----------------------------------//

PluginMetadata GizmoPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Gizmos";
	metadata.description = "Provides translate, rotate and scale tools";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void GizmoPlugin::onPluginEnable()
{
	wxToolBar* toolBar = editor->getToolbar();

	addTool( toolBar->AddSeparator() );

	wxBitmap iconCamera = wxMEMORY_BITMAP(camera);
	buttonCamera = toolBar->AddTool( GizmoTool::Camera, "Camera",
		iconCamera, "Selects the Camera View tool", wxITEM_RADIO );
	addTool(buttonCamera);

	wxBitmap iconSelect = wxMEMORY_BITMAP(cursor);
	buttonSelect = toolBar->AddTool( GizmoTool::Select, "Select",
		iconSelect, "Selects the Entity Selection tool", wxITEM_RADIO );
	addTool(buttonSelect);

	wxBitmap iconTranslate = wxMEMORY_BITMAP(move);
	buttonTranslate = toolBar->AddTool( GizmoTool::Translate, "Move",
		iconTranslate, "Selects the Move tool", wxITEM_RADIO );
	addTool(buttonTranslate);

	wxBitmap iconRotate = wxMEMORY_BITMAP(rotate2);
	buttonRotate = toolBar->AddTool( GizmoTool::Rotate, "Rotate",
		iconRotate, "Selects the Rotate tool", wxITEM_RADIO );
	addTool(buttonRotate);

	wxBitmap iconScale = wxMEMORY_BITMAP(scale);
	buttonScale = toolBar->AddTool( GizmoTool::Scale, "Scale",
		iconScale, "Selects the Scale tool", wxITEM_RADIO );
	addTool(buttonScale);

	#pragma TODO("Initialize plugins events properly")
	Events* events = editor->getEventManager();
	events->currentPlugin = this;
	tool = GizmoTool::Camera;

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&GizmoPlugin::onToolClick, this,
		buttonCamera->GetId(), buttonScale->GetId() );
}

//-----------------------------------//

void GizmoPlugin::onPluginDisable()
{
	unselectEntities();
}

//-----------------------------------//

void GizmoPlugin::onSceneLoad( const ScenePtr& scene )
{
	unselectEntities();
}

//-----------------------------------//

void GizmoPlugin::onToolClick(wxCommandEvent& event)
{
	tool = (GizmoTool::Enum) event.GetId();

	unselectEntities(true);
}

//-----------------------------------//

void GizmoPlugin::unselectEntities(bool reselect)
{
	Events* events = editor->getEventManager();

	std::vector<EntityPtr> nodes;

	GizmoMap::const_iterator it;
	for( it = gizmos.cbegin(); it != gizmos.cend(); it++ )
	{
		nodes.push_back(it->first);
	}

	for( uint i = 0; i < nodes.size(); i++ )
	{
		const EntityPtr& node = nodes[i];
		events->onEntityUnselect(node);

		if( reselect )
			onEntitySelect(node);
	}
}

//-----------------------------------//

void GizmoPlugin::onEntitySelect( const EntityPtr& node )
{
	editor->redrawView();

	unselectEntities();

	if( isTool(GizmoTool::Camera) )
		return;

	setBoundingBoxVisible( node, true );

	if( isTool(GizmoTool::Select) )
	{
		gizmos[node] = EntityPtr();
		return;
	}
	
	bool gizmoExists = gizmos.find(node) != gizmos.end();
	
	if( !gizmoExists )
			createGizmo( node );
}

//-----------------------------------//

void GizmoPlugin::onEntityUnselect( const EntityPtr& node )
{
	setBoundingBoxVisible( node, false );
	removeGizmo( node );

	editor->redrawView();
}

//-----------------------------------//

Plane GizmoPlugin::getGizmoPickPlane()
{
	const EntityPtr nodeObject( op->weakEntity );
	const TransformPtr& transObject = nodeObject->getTransform();

	Vector3 gizmoAxis = gizmo->getAxisVector(op->axis);
	Vector3 gizmoPosition = transObject->getPosition();
	
	Plane planeX( Vector3::UnitX, gizmoPosition.x );
	Plane planeY( Vector3::UnitY, gizmoPosition.y );
	Plane planeZ( Vector3::UnitZ, gizmoPosition.z );

	RenderView* renderView = viewframe->getView();
	const CameraPtr& camera = renderView->getCamera();
	
	Vector2i viewCenter = renderView->getSize() / 2.0f;
	Ray ray = camera->getRay(viewCenter.x, viewCenter.y);
	
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
	RenderView* renderView = viewframe->getView();
	const CameraPtr& camera = renderView->getCamera();	

	Plane pickPlane = getGizmoPickPlane();	
	Ray ray = camera->getRay(x, y);
	
	float distance;
	if( !pickPlane.intersects(ray, distance) )
		return false;

	pickPoint = ray.getPoint(distance);
	
	return true;
}

//-----------------------------------//

bool GizmoPlugin::getGizmoPickEntity(int x, int y ,EntityPtr& node)
{
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
	RayBoxQueryResult res;

	if( !scene->doRayBoxQuery(pickRay, res) )
		return false;

	node = res.node;

	return true;
}

//-----------------------------------//

void GizmoPlugin::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !gizmo )
		return;

	if( !(isTool(GizmoTool::Translate)
		|| isTool(GizmoTool::Scale) || isTool(GizmoTool::Rotate)) )
		return;

	// To check if the user picked a gizmo we use two hit test techniques.
	// The first technique is based on bounding volumes so it is not very
	// accurate. If it hits, a more accurate color picking will be done.

	if( pickBoundingTest(moveEvent) && pickImageTest(moveEvent, axis) )
	{
		gizmo->selectAxis(axis);
		
		if( !op )
			createOperation();
		else
			op->axis = axis;
	}
	else
	{
		gizmo->deselectAxis();
		
		delete op;
		op = nullptr;
	}

	editor->redrawView();
}

//-----------------------------------//

void GizmoPlugin::onMouseDrag( const MouseDragEvent& dragEvent )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	if( !op )
		return;
	
	Vector3 pickPoint;
	
	if( !getGizmoPickPoint(dragEvent.x, dragEvent.y, pickPoint) )
		return;

	Vector3 pickAxis = gizmo->getAxisVector(op->axis);
	Vector3 pickOffset = pickAxis.project(pickPoint - firstPickPoint);

	const EntityPtr nodeObject( op->weakEntity );
	const TransformPtr& transObject = nodeObject->getTransform();
	
	if( isTool(GizmoTool::Translate) )
		transObject->translate( (pickPoint - firstPickPoint)*pickAxis );

	firstPickPoint = pickPoint;

	//else if( isTool(GizmoTool::Scale) )
		//transObject->scale( Vector3(1, 1, 1)+unit*0.05f );

	op->scale = transObject->getScale();
	op->rotation = transObject->getRotation();
	op->translation = transObject->getPosition();

	editor->redrawView();
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( isTool(GizmoTool::Camera) )
		return;

	if( mbe.button != MouseButton::Left )
		return;
	
	if( gizmo && gizmo->isAnyAxisSelected() )
	{
		getGizmoPickPoint(mbe.x, mbe.y, firstPickPoint);
		return;
	}
	
	unselectEntities();

	EntityPtr node;
	if( !getGizmoPickEntity(mbe.x, mbe.y, node) )
		return;

	Events* events = editor->getEventManager();
	events->onEntitySelect(node);
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonRelease( const MouseButtonEvent& mbe )
{
	if( !op )
		return;

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation( op );
	op = nullptr;
	
	gizmo->deselectAxis();
}

//-----------------------------------//

void GizmoPlugin::createGizmo( const EntityPtr& node )
{
	assert( node != nullptr );
	assert( gizmos.find(node) == gizmos.end() );

	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera();

	Gizmo* newGizmo = nullptr;

	// Create new Gizmo for the node.
	if( isTool(GizmoTool::Translate) )
		newGizmo = new GizmoTranslate(node, camera);

	else if( isTool(GizmoTool::Rotate) )
		newGizmo = new GizmoRotate(node, camera);

	else if( isTool(GizmoTool::Scale) )
		newGizmo = new GizmoScale(node, camera);

	else assert( 0 && "Unknown gizmo tool" );

	gizmo.reset(newGizmo);
	gizmo->buildGeometry();

	// Add the gizmo to the scene.
	EntityPtr nodeGizmo( new Entity("Gizmo") );
	nodeGizmo->addTransform();
	nodeGizmo->addComponent(gizmo);
	editorScene->add(nodeGizmo);

	gizmo->updatePositionScale();
	
	// Associate the gizmo with the node.
	gizmos[node] = nodeGizmo;
}

//-----------------------------------//

void GizmoPlugin::removeGizmo( const EntityPtr& node )
{
	assert( node != nullptr );

	// Find the existing gizmo associated with the node.
	GizmoMap::iterator it = gizmos.find(node);

	if( it == gizmos.end() )
		return;

	// Find the node of the gizmo.
	const EntityPtr& nodeGizmo = gizmos[node];
	
	// Remove the gizmo.
	editorScene->remove(nodeGizmo);
	gizmos.erase(it);
	gizmo.reset();
}

//-----------------------------------//

void GizmoPlugin::setBoundingBoxVisible( const EntityPtr& node, bool state )
{
	assert( node != nullptr );

	const TransformPtr& transform = node->getTransform();

	if( !transform )
		return;

	transform->setDebugRenderableVisible( state );
}

//-----------------------------------//

bool GizmoPlugin::pickImageTest( const MouseMoveEvent& moveEvent,
								 GizmoAxis::Enum& axis )
{
	RenderView* view = viewframe->getView();
	Vector2i size = view->getSize();

	// We need to flip the Y-axis due to a mismatch between the 
	// OpenGL and wxWidgets coordinate-system origin conventions.
	int mouseX = moveEvent.x;
	int mouseY = size.y - moveEvent.y;

	RenderDevice* device = engine->getRenderDevice();
	Color pick = device->getPixel(mouseX, mouseY);

	axis = gizmo->getAxis(pick);
	return axis != GizmoAxis::None;
}

//-----------------------------------//

bool GizmoPlugin::pickBoundingTest( const MouseMoveEvent& me )
{
	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( me.x, me.y, &outFar );

	// Perform ray casting to find the nodes.
	RayBoxQueryResult res;
	
	if( !editorScene->doRayBoxQuery(pickRay, res) )
		return false;

	// Find out if we picked a gizmo...
	GizmoPtr gizmo = res.node->getComponent<Gizmo>();
	
	return gizmo != nullptr;
}

//-----------------------------------//

void GizmoPlugin::createOperation()
{
	assert( op == nullptr );
	
	EntityPtr nodeObject = gizmo->getEntityAttachment();
	TransformPtr transObject = nodeObject->getTransform();

	op = new GizmoOperation();

	op->tool = tool;
	op->axis = axis;
	op->gizmo = gizmo;
	op->weakEntity = nodeObject;

	op->prevTranslation = transObject->getPosition();
	op->prevScale = transObject->getScale();
	op->prevRotation = transObject->getRotation();
}

//-----------------------------------//

bool GizmoPlugin::isTool(GizmoTool::Enum mode)
{
	return mode == tool;
}

//-----------------------------------//

} } // end namespaces