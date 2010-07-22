/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "GizmoTool.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "vapor/render/DebugGeometry.h"

namespace vapor { namespace editor {

//-----------------------------------//

GizmoTool::GizmoTool( EditorFrame* frame )
	: Tool(frame)
	, editorScene(frame->getEditorScene())
	, op(nullptr)
{
	assert( editorScene != nullptr );
}

//-----------------------------------//

void GizmoTool::onToolInit(wxToolBar* toolBar, ToolsMap& modes)
{
	toolBar->AddSeparator();

	toolBar->AddTool( GizmoType::Camera, "Camera", wxMEMORY_BITMAP(camera), 
		"Selects the Camera View tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoType::Select, "Select", wxMEMORY_BITMAP(cursor), 
		"Selects the Entity Selection tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoType::Translate, "Move", wxMEMORY_BITMAP(move), 
		"Selects the Move tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoType::Rotate, "Rotate", wxMEMORY_BITMAP(rotate2), 
		"Selects the Rotate tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoType::Scale, "Scale", wxMEMORY_BITMAP(scale), 
		"Selects the Scale tool", wxITEM_RADIO );

	// Register the modes.
	modes[GizmoType::Camera] = this;
	modes[GizmoType::Select] = this;
	modes[GizmoType::Translate] = this;
	modes[GizmoType::Rotate] = this;
	modes[GizmoType::Scale] = this;
}

//-----------------------------------//

void GizmoTool::onToolEnable( int id )
{
	tool = (GizmoType::Enum) id;
}

//-----------------------------------//

void GizmoTool::onToolDisable()
{
	disableSelectedNodes();
}

//-----------------------------------//

void GizmoTool::createGizmo( const NodePtr& node )
{
	assert( node != nullptr );
	assert( gizmos.find(node) == gizmos.end() );

	// Create new Gizmo for the node.
	const CameraPtr& camera = viewframe->getCamera();
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

void GizmoTool::removeGizmo( const NodePtr& node )
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

void GizmoTool::setBoundingBoxVisible( const NodePtr& node, bool state )
{
	assert( node != nullptr );

	const TransformPtr& transform = node->getTransform();

	if( !transform )
		return;

	transform->setDebugRenderableVisible( state );
}

//-----------------------------------//

void GizmoTool::disableSelectedNodes()
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

void GizmoTool::drawGizmo( NodePtr nodeOld, NodePtr nodeNew )
{
	if( !isMode(GizmoType::Translate) )
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

void GizmoTool::onNodeSelected( NodePtr nodeOld, NodePtr nodeNew )
{
	assert( nodeNew != nullptr ); 

	if( isMode(GizmoType::Camera) )
		return;
	
	if( isMode(GizmoType::Select) )
	{
		if( nodeOld )
			setBoundingBoxVisible( nodeOld, false );

		setBoundingBoxVisible( nodeNew, true );
		return;
	}

	if( isMode(GizmoType::Translate) || isMode(GizmoType::Scale) || isMode(GizmoType::Rotate) )
	{
		drawGizmo( nodeOld, nodeNew );
	}
}

//-----------------------------------//

void GizmoTool::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if( !gizmo )
		return;

	if( !isMode(GizmoType::Translate) )
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

void GizmoTool::onMouseDrag( const MouseDragEvent& dragEvent )
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

void GizmoTool::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( gizmo && gizmo->isAnyAxisSelected() )
		return;

	const ScenePtr& scene = engine->getSceneManager();
	const CameraPtr& camera = viewframe->getCamera();

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
		onNodeSelected( NodePtr(), res.node );
	//else
		//disableSelectedNodes();
}

//-----------------------------------//

void GizmoTool::onMouseButtonRelease( const MouseButtonEvent& mbe )
{
	if( !op )
		return;

	editor->registerOperation( op );
	op = nullptr;
}

//-----------------------------------//

bool GizmoTool::pickImageTest( const MouseMoveEvent& moveEvent, GizmoAxis::Enum& axis )
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

bool GizmoTool::pickBoundingTest( const MouseMoveEvent& me )
{
	const CameraPtr& camera = viewframe->getCamera();

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

void GizmoTool::createOperation()
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
	: tool(GizmoType::Camera)
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
	assert( tool == GizmoType::Translate );

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