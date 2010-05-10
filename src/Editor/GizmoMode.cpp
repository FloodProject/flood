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
	: Mode( frame ), editorScene( frame->getEditorScene() ),
	op( nullptr )
{
	assert( editorScene != nullptr );
}

//-----------------------------------//

void GizmoMode::onModeInit(wxToolBar* toolBar, ModeIdMap& map)
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

	map[GizmoTool::Camera] = this;
	map[GizmoTool::Select] = this;
	map[GizmoTool::Translate] = this;
	map[GizmoTool::Rotate] = this;
	map[GizmoTool::Scale] = this;
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

	const TransformPtr& tr = node->getTransform();
	tr->setDebugRenderableVisible( true );

	static int i = 0;
	std::string str( "Gizmo"+num_to_str(i++) );
	NodePtr gizNode( new Node(str) );
	gizNode->addTransform();

	gizmo.reset( new Gizmo(node) );
	gizNode->addComponent(gizmo);
	
	editorScene->add( gizNode );
	gizmos[node] = gizNode;
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

	const NodePtr& gizNode = gizmos[node];
	
	editorScene->remove(gizNode);
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

void GizmoMode::drawGizmo( NodePtr old, NodePtr _new )
{
	if( tool != GizmoTool::Translate )
		return;

	if( old )
		disableBoundingGizmo( old );

	if( _new && ( gizmos.find(_new) == gizmos.end() ) )
		enableBoundingGizmo( _new );

	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::onNodeSelected( NodePtr old, NodePtr _new )
{
	assert( _new != nullptr ); 

	switch(tool) 
	{	
		case GizmoTool::Select:
		{
			TransformPtr transform = _new->getTransform();

			if( transform )
				transform->setDebugRenderableVisible( true );
			
			break;
		}

		case GizmoTool::Translate:
		case GizmoTool::Rotate:
		case GizmoTool::Scale:
		{
			drawGizmo( old, _new );
			break;
		} 
	}

	selected.push_back( _new );
}

//-----------------------------------//

void GizmoMode::onMouseMove( const MouseMoveEvent& me )
{
	if( ((tool != GizmoTool::Translate)
		&& (tool != GizmoTool::Rotate)
		&& (tool != GizmoTool::Scale))
		|| !gizmo )
	{
		return;
	}

	// To check if the user picked a gizmo we use two hit tests.
	// The first will be based on bounding volumes and will be
	// a conservative but not very accurate first result.
	// If it hits, then we will test with a more accurate
	// (but also more expensive) image based picking technique.

	//if( !pickBoundingTest(me) )
	//{
	//	gizmo->setAxisDefault();
	//	return;
	//}

	if( pickImageTest(me, axis) )
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

void GizmoMode::onMouseDrag( const MouseDragEvent& de )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	assert( op != nullptr );

	Vector3 unit = Gizmo::getUnitVector(op->axis);

	if( axis == GizmoAxis::X )
		unit *= de.dx;
	else if( axis == GizmoAxis::Y )
		unit *= de.dy;
	else if( axis == GizmoAxis::Z )
		unit *= de.dx;

	const NodePtr node( op->weakNode );
	const TransformPtr& transform = node->getTransform();
	transform->translate( unit );
	
	const NodePtr& giz_node = gizmo->getNode();
	const TransformPtr& giz_tr = giz_node->getTransform();
	giz_tr->translate( unit );

	op->translation = transform->getPosition();
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

	if( scene->doRayBoxQuery( pickRay, res ) )
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

bool GizmoMode::pickImageTest( const MouseMoveEvent& me, GizmoAxis::Enum& axis )
{
	ViewportPtr viewport = viewframe->getViewport();
	Vector2i size = viewport->getSize();

	// We need to flip the Y-axis due to a mismatch between the 
	// OpenGL and wxWidgets coordinate-system origin conventions.
	int m_x = me.x;
	int m_y = size.y-me.y;

	const render::DevicePtr& device = engine->getRenderDevice();
	Color pick = device->getPixel(m_x, m_y);
	
	axis = Gizmo::getAxis(pick);
	
	return ( axis != GizmoAxis::None );
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
	: tool(GizmoTool::Camera),
	scale(1.0f, 1.0f, 1.0f)
{
}

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

	TransformPtr transform = node->getTransform();

	const Vector3& tr = undo ?
		orig_translation : translation;

	transform->setPosition( tr );

	// Modify the gizmo if it still exists.
	NodePtr giz_node( gizmo->getNode() );
	
	if( !giz_node )
		return;

	TransformPtr giz_tr = giz_node->getTransform();

	if( giz_tr )
		giz_tr->setPosition( tr );
}

//-----------------------------------//

} } // end namespaces