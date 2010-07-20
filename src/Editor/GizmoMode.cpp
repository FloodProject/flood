/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "EditorIcons.h"
#include "GizmoMode.h"
#include "Editor.h"
#include "vaporWindow.h"
#include "vapor/render/DebugGeometry.h"

namespace vapor { namespace editor {

//-----------------------------------//

GizmoMode::GizmoMode( EditorFrame* frame )
	: Mode(frame)
	, editorScene(frame->getEditorScene())
	, op(nullptr)
{
	assert( editorScene != nullptr );
}

//-----------------------------------//

void GizmoMode::onModeInit(wxToolBar* toolBar, ModeIdMap& modes)
{
	toolBar->AddSeparator();

	toolBar->AddTool( GizmoTool::Camera, "Camera", wxMEMORY_BITMAP(camera), 
		"Selects the Camera View tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoTool::Select, "Select", wxMEMORY_BITMAP(cursor), 
		"Selects the Entity Selection tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoTool::Translate, "Move", wxMEMORY_BITMAP(move), 
		"Selects the Move tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoTool::Rotate, "Rotate", wxMEMORY_BITMAP(rotate2), 
		"Selects the Rotate tool", wxITEM_RADIO );

	toolBar->AddTool( GizmoTool::Scale, "Scale", wxMEMORY_BITMAP(scale), 
		"Selects the Scale tool", wxITEM_RADIO );

	modes[GizmoTool::Camera] = this;
	modes[GizmoTool::Select] = this;
	modes[GizmoTool::Translate] = this;
	modes[GizmoTool::Rotate] = this;
	modes[GizmoTool::Scale] = this;
}

//-----------------------------------//

void GizmoMode::onModeEnter( int id )
{
	tool = (GizmoTool::Enum) id;
}

//-----------------------------------//

void GizmoMode::onModeExit()
{
	disableSelectedNodes();
}

//-----------------------------------//

void GizmoMode::enableBoundingGizmo( const NodePtr& node )
{
	assert( node != nullptr );
	assert( gizmos.find(node) == gizmos.end() );

	const TransformPtr& transNode = node->getTransform();
	transNode->setDebugRenderableVisible( true );

	gizmo.reset( new Gizmo(node) );

	static int i = 0;
	std::string str( "Gizmo"+String::fromNumber(i++) );
	NodePtr nodeGizmo( new Node(str) );
	nodeGizmo->addTransform();
	nodeGizmo->addComponent(gizmo);
	editorScene->add( nodeGizmo );
	
	gizmos[node] = nodeGizmo;
}

//-----------------------------------//

void GizmoMode::disableBoundingGizmo( const NodePtr& node )
{
	assert( node != nullptr );

	const TransformPtr& tr = node->getTransform();

	if( !tr )
		return;

	tr->setDebugRenderableVisible( false );

	GizmoNodeMap::iterator it = gizmos.find(node);
	
	if( it == gizmos.end() )
		return;

	const NodePtr& nodeGizmo = gizmos[node];
	
	editorScene->remove(nodeGizmo);
	gizmos.erase(it);
	gizmo.reset();
}

//-----------------------------------//

void GizmoMode::disableSelectedNodes()
{
	// Disable all enabled bounding boxes.
	foreach( const NodePtr& node, selected )
		disableBoundingGizmo( node );

	selected.clear();
	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::drawGizmo( NodePtr nodeOld, NodePtr nodeNew )
{
	if( tool != GizmoTool::Translate )
		return;

	if( nodeOld )
		disableBoundingGizmo( nodeOld );

	bool newGizmoNotExists = gizmos.find(nodeNew) == gizmos.end();
	
	if( nodeNew && newGizmoNotExists )
		enableBoundingGizmo( nodeNew );

	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::onNodeSelected( NodePtr nodeOld, NodePtr nodeNew )
{
	assert( nodeNew != nullptr ); 

	if( tool == GizmoTool::Camera )
	{

	}
	else if( tool == GizmoTool::Select )
	{
		TransformPtr transform = nodeNew->getTransform();

		if( transform )
			transform->setDebugRenderableVisible(true);
	}
	else
	{
		drawGizmo( nodeOld, nodeNew ); 
	}

	selected.push_back( nodeNew );
}

//-----------------------------------//

void GizmoMode::onMouseMove( const MouseMoveEvent& moveEvent )
{
	if(!gizmo)
		return;
	
	if(!isMode(GizmoTool::Translate) 
		|| !isMode(GizmoTool::Rotate) || !isMode(GizmoTool::Scale))
		return;

	// To check if the user picked a gizmo we use two hit tests.
	// First a not very accurate test based on bounding volumes.
	// If it hits, then we will test with a more accurate image
	// based picking technique (more expensive test).

	//if( !pickBoundingTest(me) )
	//{
	//	gizmo->setAxisDefault();
	//	return;
	//}

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

void GizmoMode::onMouseDrag( const MouseDragEvent& dragEvent )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	assert( op != nullptr );

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
	const NodePtr& nodeGizmo = gizmo->getNode();
	const TransformPtr& transGizmo = nodeGizmo->getTransform();
	transGizmo->translate( unit );

	op->translation = transObject->getPosition();
	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::onMouseButtonPress( const MouseButtonEvent& mbe )
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
	{
		onNodeSelected( NodePtr(), res.node );
	}
	else
	{
		disableSelectedNodes();
	}
}

//-----------------------------------//

void GizmoMode::onMouseButtonRelease( const MouseButtonEvent& mbe )
{
	if( !op )
		return;

	editor->registerOperation( op );
	op = nullptr;
}

//-----------------------------------//

bool GizmoMode::pickImageTest( const MouseMoveEvent& moveEvent, GizmoAxis::Enum& axis )
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

bool GizmoMode::pickBoundingTest( const MouseMoveEvent& me )
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

void GizmoMode::createOperation()
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