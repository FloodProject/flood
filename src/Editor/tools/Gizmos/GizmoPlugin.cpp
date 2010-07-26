/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoPlugin.h"
#include "Editor.h"
#include "UndoManager.h"
#include "EditorIcons.h"
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

void GizmoPlugin::onPluginEnable(wxToolBar* toolBar)
{
	toolBar->AddSeparator();

	wxBitmap iconCamera = wxMEMORY_BITMAP(camera);
	buttonCamera = toolBar->AddTool( wxID_ANY, "Camera",
		iconCamera, "Selects the Camera View tool", wxITEM_RADIO );

	wxBitmap iconSelect = wxMEMORY_BITMAP(cursor);
	buttonSelect = toolBar->AddTool( GizmoTool::Select, "Select",
		iconSelect, "Selects the Entity Selection tool", wxITEM_RADIO );

	wxBitmap iconTranslate = wxMEMORY_BITMAP(move);
	buttonTranslate = toolBar->AddTool( GizmoTool::Translate, "Move",
		iconTranslate, "Selects the Move tool", wxITEM_RADIO );

	wxBitmap iconRotate = wxMEMORY_BITMAP(rotate2);
	buttonRotate = toolBar->AddTool( GizmoTool::Rotate, "Rotate",
		iconRotate, "Selects the Rotate tool", wxITEM_RADIO );

	wxBitmap iconScale = wxMEMORY_BITMAP(scale);
	buttonScale = toolBar->AddTool( GizmoTool::Scale, "Scale",
		iconScale, "Selects the Scale tool", wxITEM_RADIO );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&GizmoPlugin::onGizmoButtonClick, this,
		buttonCamera->GetId(), buttonScale->GetId() );
}

//-----------------------------------//

void GizmoPlugin::onPluginDisable(wxToolBar* toolBar)
{
	int id;
	
	id = buttonCamera->GetId();
	toolBar->DeleteTool(id);

	id = buttonSelect->GetId();
	toolBar->DeleteTool(id);

	id = buttonTranslate->GetId();
	toolBar->DeleteTool(id);

	id = buttonRotate->GetId();
	toolBar->DeleteTool(id);

	id = buttonScale->GetId();
	toolBar->DeleteTool(id);

	disableSelectedNodes();
}

//-----------------------------------//

void GizmoPlugin::onGizmoButtonClick(wxCommandEvent& event)
{
	//tool
}

//-----------------------------------//

void GizmoPlugin::onNodeSelect( const NodePtr& node )
{
	//if( isMode(GizmoTool::Camera) )
	//	return;
	//
	//if( isMode(GizmoTool::Select) )
	//{
	//	if( nodeOld )
	//		setBoundingBoxVisible( nodeOld, false );

	//	setBoundingBoxVisible( nodeNew, true );
	//	return;
	//}

	//if( isMode(GizmoTool::Translate) || isMode(GizmoTool::Scale) || isMode(GizmoTool::Rotate) )
	//{
	//	drawGizmo( nodeOld, nodeNew );
	//}
	setBoundingBoxVisible( node, true );
	editor->RefreshViewport();
}

//-----------------------------------//

void GizmoPlugin::onNodeUnselect( const NodePtr& node )
{
	setBoundingBoxVisible( node, false );
	editor->RefreshViewport();
}

//-----------------------------------//

void GizmoPlugin::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !gizmo )
		return;

	if( !isMode(GizmoTool::Translate) )
		return;

	// To check if the user picked a gizmo we use two hit tests.
	// First a not very accurate test based on bounding volumes.
	// If it hits, then we will test with a more accurate image
	// based picking technique (more expensive test).

	if( !pickBoundingTest(moveEvent) )
	{
		// TODO: Uncomment once picking works properly.
		//gizmo->deselectAxis();
		//return;
	}

	if( pickImageTest(moveEvent, axis) )
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
	
	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoPlugin::onMouseDrag( const MouseDragEvent& dragEvent )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	//assert( op != nullptr );

	if( !op )
		return;

	Vector3 unit = Gizmo::getUnitVector(op->axis);

	if( axis == GizmoAxis::X )
		unit *= dragEvent.dx;

	else if( axis == GizmoAxis::Y )
		unit *= dragEvent.dy;

	else if( axis == GizmoAxis::Z )
		unit *= dragEvent.dx;

	// Transform the object.
	const NodePtr nodeObject( op->weakNode );
	const TransformPtr& transObject = nodeObject->getTransform();
	transObject->translate( unit );
	
	// Transform the gizmo.
	//const NodePtr& nodeGizmo = gizmo->getNode();
	//const TransformPtr& transGizmo = nodeGizmo->getTransform();
	//transGizmo->translate( unit );

	op->translation = transObject->getPosition();
	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( gizmo && gizmo->isAnyAxisSelected() )
		return;

	Viewport* viewport = viewframe->getViewport();
	const CameraPtr& camera = viewport->getCamera();
	const ScenePtr& scene = engine->getSceneManager();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( mbe.x, mbe.y, &outFar );

#if 1 // Enable this to draw debugging lines
	const NodePtr& line = buildRay( pickRay, outFar );
	editor->getEditorScene()->add( line );
#endif

	// Perform ray casting to find the nodes.
	RayBoxQueryResult res;

	if( scene->doRayBoxQuery(pickRay, res) )
		onNodeSelect( res.node );
	//else
		//disableSelectedNodes();
}

//-----------------------------------//

void GizmoPlugin::onMouseButtonRelease( const MouseButtonEvent& mbe )
{
	if( !op )
		return;

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation( op );

	op = nullptr;
}

//-----------------------------------//

void GizmoPlugin::createGizmo( const NodePtr& node )
{
	assert( node != nullptr );
	assert( gizmos.find(node) == gizmos.end() );

	Viewport* viewport = viewframe->getViewport();
	const CameraPtr& camera = viewport->getCamera();

	// Create new Gizmo for the node.
	gizmo.reset( new Gizmo(node, camera) );

	// Add the gizmo to the scene.
	NodePtr nodeGizmo( new Node("Gizmo") );
	nodeGizmo->addTransform();
	nodeGizmo->addComponent(gizmo);
	editorScene->add(nodeGizmo);
	
	// Associate the gizmo with the node.
	gizmos[node] = nodeGizmo;
}

//-----------------------------------//

void GizmoPlugin::removeGizmo( const NodePtr& node )
{
	assert( node != nullptr );

	// Find the existing gizmo associated with the node.
	GizmoNodeMap::iterator it = gizmos.find(node);
	//assert( it != gizmos.end() );

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

void GizmoPlugin::disableSelectedNodes()
{
	// Disable all selected nodes bounding boxes and gizmos.
	foreach( const NodePtr& node, selected )
	{
		removeGizmo( node );
		setBoundingBoxVisible( node, false );
	}

	selected.clear();
	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoPlugin::drawGizmo( NodePtr nodeOld, NodePtr nodeNew )
{
	if( !isMode(GizmoTool::Translate) )
		return;

	if( nodeOld )
	{
		removeGizmo( nodeOld );
		setBoundingBoxVisible( nodeOld, false );
	}

	bool newGizmoNotExists = gizmos.find(nodeNew) == gizmos.end();
	
	if( nodeNew && newGizmoNotExists )
	{
		createGizmo( nodeNew );
		setBoundingBoxVisible( nodeNew, true );
	}

	viewframe->flagRedraw();
}

//-----------------------------------//

bool GizmoPlugin::pickImageTest( const MouseMoveEvent& moveEvent, GizmoAxis::Enum& axis )
{
	Viewport* viewport = viewframe->getViewport();
	Vector2i size = viewport->getSize();

	// We need to flip the Y-axis due to a mismatch between the 
	// OpenGL and wxWidgets coordinate-system origin conventions.
	int mouseX = moveEvent.x;
	int mouseY = size.y-moveEvent.y;

	RenderDevice* device = engine->getRenderDevice();
	Color pick = device->getPixel(mouseX, mouseY);
	
	axis = Gizmo::getAxis(pick);
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
	gizmo = res.node->getComponent<Gizmo>("Gizmo");
	
	return (gizmo != nullptr);
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

	NodePtr node = gizmo->getNodeAttachment();
	TransformPtr tr = node->getTransform();

	op->orig_translation = tr->getPosition();
	op->orig_scale = tr->getScale();
	op->orig_rotation = tr->getRotation();
}

//-----------------------------------//

GizmoOperation::GizmoOperation()
	: tool(GizmoTool::Camera)
	, scale(1.0f, 1.0f, 1.0f)
{ }

//-----------------------------------//

void GizmoOperation::undo()
{
	process( true );
}

//-----------------------------------//

void GizmoOperation::redo()
{
	process( false );
}

//-----------------------------------//

void GizmoOperation::process( bool undo )
{
	assert( tool == GizmoTool::Translate );

	NodePtr node( weakNode );

	// This can happen if the node gets deleted between
	// the operation registration and the undo/redo action.
	if( !node )
		return;

	const Vector3& tr = undo ? orig_translation : translation;

	TransformPtr transform = node->getTransform();
	transform->setPosition( tr );

	// Modify the gizmo if it still exists.
	NodePtr nodeGizmo( gizmo->getNode() );
	
	if( !nodeGizmo )
		return;

	TransformPtr transGizmo = nodeGizmo->getTransform();

	if( transGizmo )
		transGizmo->setPosition( tr );
}

//-----------------------------------//

} } // end namespaces