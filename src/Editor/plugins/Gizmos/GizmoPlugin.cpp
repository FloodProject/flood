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
{
	assert( editorScene != nullptr );
}

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

	// TODO: Hack.
	Events* events = editor->getEventManager();
	events->currentPlugin = this;

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&GizmoPlugin::onGizmoToolClick, this,
		buttonCamera->GetId(), buttonScale->GetId() );
}

//-----------------------------------//

void GizmoPlugin::onPluginDisable()
{
	unselectNodes();
}

//-----------------------------------//

void GizmoPlugin::onGizmoToolClick(wxCommandEvent& event)
{
	tool = (GizmoTool::Enum) event.GetId();

	unselectNodes(true);
}

//-----------------------------------//

void GizmoPlugin::unselectNodes(bool reselect)
{
	std::vector<NodePtr> nodes;
	
	foreach( const GizmoMapPair& pair, gizmos )
		nodes.push_back(pair.first);

	foreach( const NodePtr& node, nodes )
	{
		onNodeUnselect(node);

		if( reselect )
			onNodeSelect(node);
	}
}

//-----------------------------------//

void GizmoPlugin::onNodeSelect( const NodePtr& node )
{
	setBoundingBoxVisible( node, true );

	if( isTool(GizmoTool::Translate)
		|| isTool(GizmoTool::Scale) || isTool(GizmoTool::Rotate) )
	{
		bool gizmoExists = gizmos.find(node) != gizmos.end();
	
		if( !gizmoExists )
			createGizmo( node );
	}
	else
	{
		gizmos[node] = NodePtr();
	}

	editor->RefreshViewport();
}

//-----------------------------------//

void GizmoPlugin::onNodeUnselect( const NodePtr& node )
{
	setBoundingBoxVisible( node, false );
	removeGizmo( node );

	editor->RefreshViewport();
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

	editor->RefreshViewport();
}

//-----------------------------------//

void GizmoPlugin::onMouseDrag( const MouseDragEvent& dragEvent )
{
	//if( isTool(GizmoTool::Camera) )
	//{
	//	Viewport* viewport = viewframe->getViewport();
	//	CameraPtr camera = viewport->getCamera();
	//	NodePtr nodeCamera = camera->getNode();
	//	TransformPtr transCamera = nodeCamera->getTransform();
	//	transCamera->translate( dragEvent.dx, dragEvent.dy, 0 );
	//	return;
	//}

	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	if( !op )
		return;

	Vector3 unit = gizmo->getAxisVector(op->axis);

	if( axis == GizmoAxis::X )
		unit *= dragEvent.dx;

	else if( axis == GizmoAxis::Y )
		unit *= dragEvent.dy;

	else if( axis == GizmoAxis::Z )
		unit *= dragEvent.dx;

	// Transform the object.
	const NodePtr nodeObject( op->weakNode );
	const TransformPtr& transObject = nodeObject->getTransform();

	if( isTool(GizmoTool::Translate) )
		transObject->translate( unit );
	else if( isTool(GizmoTool::Scale) )
		transObject->scale( Vector3(1, 1, 1)+unit*0.05f );

	op->scale = transObject->getScale();
	op->rotation = transObject->getRotation();
	op->translation = transObject->getPosition();

	editor->RefreshViewport();
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( mbe.button != MouseButton::Left )
		return;

	if( gizmo && gizmo->isAnyAxisSelected() )
		return;

	Viewport* viewport = viewframe->getViewport();
	const CameraPtr& camera = viewport->getCamera();
	const ScenePtr& scene = engine->getSceneManager();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( mbe.x, mbe.y, &outFar );

#if 0 // Enable this to draw debugging lines
	const NodePtr& line = buildRay( pickRay, outFar );
	editor->getEditorScene()->add( line );
#endif

	// Perform ray casting to find the nodes.
	RayBoxQueryResult res;

	if( scene->doRayBoxQuery(pickRay, res) )
		onNodeSelect( res.node );
	else
		unselectNodes();
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

void GizmoPlugin::createGizmo( const NodePtr& node )
{
	assert( node != nullptr );
	assert( gizmos.find(node) == gizmos.end() );

	Viewport* viewport = viewframe->getViewport();
	const CameraPtr& camera = viewport->getCamera();

	Gizmo* newGizmo = nullptr;

	// Create new Gizmo for the node.
	if( isTool(GizmoTool::Translate) )
		newGizmo = new GizmoTranslate(node, camera);

	else if( isTool(GizmoTool::Rotate) )
		newGizmo = new GizmoRotate(node, camera);

	else if( isTool(GizmoTool::Scale) )
		newGizmo = new GizmoScale(node, camera);

	gizmo.reset(newGizmo);
	gizmo->buildGeometry();

	// Add the gizmo to the scene.
	NodePtr nodeGizmo( new Node("Gizmo") );
	nodeGizmo->addTransform();
	nodeGizmo->addComponent(gizmo);
	editorScene->add(nodeGizmo);

	gizmo->updatePositionScale();
	
	// Associate the gizmo with the node.
	gizmos[node] = nodeGizmo;
}

//-----------------------------------//

void GizmoPlugin::removeGizmo( const NodePtr& node )
{
	assert( node != nullptr );

	// Find the existing gizmo associated with the node.
	GizmoMap::iterator it = gizmos.find(node);

	if( it == gizmos.end() )
		return;

	// Find the node of the gizmo.
	const NodePtr& nodeGizmo = gizmos[node];
	
	// Remove the gizmo.
	editorScene->remove(nodeGizmo);
	gizmos.erase(it);
	gizmo.reset();
}

//-----------------------------------//

void GizmoPlugin::setBoundingBoxVisible( const NodePtr& node, bool state )
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
	Viewport* viewport = viewframe->getViewport();
	Vector2i size = viewport->getSize();

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
	Viewport* viewport = viewframe->getViewport();
	const CameraPtr& camera = viewport->getCamera();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( me.x, me.y, &outFar );

	// Perform ray casting to find the nodes.
	RayBoxQueryResult res;
	
	if( !editorScene->doRayBoxQuery(pickRay, res) )
		return false;

	// Find out if we picked a gizmo...
	GizmoPtr gizmo = res.node->getComponent<Gizmo>("Gizmo");
	
	return gizmo != nullptr;
}

//-----------------------------------//

void GizmoPlugin::createOperation()
{
	assert( op == nullptr );
	
	op = new GizmoOperation();
	
	op->axis = axis;
	op->weakNode = gizmo->getNodeAttachment();
	op->tool = tool;
	op->gizmo = gizmo;

	NodePtr nodeObject = gizmo->getNodeAttachment();
	TransformPtr transObject = nodeObject->getTransform();

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