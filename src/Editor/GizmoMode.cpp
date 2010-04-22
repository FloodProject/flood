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

namespace vapor { namespace editor {

//-----------------------------------//

enum GizmoTools
{
	Gizmo_Camera = 18237,
	Gizmo_Select,
	Gizmo_Translate,
	Gizmo_Rotate,
	Gizmo_Scale
};

static const int PICK_TARGET_SIZE = 256;

//-----------------------------------//

GizmoMode::GizmoMode( EditorFrame* frame )
	: Mode( frame ), editorScene( frame->getEditorScene() ),
	oldMouseEvent(0, 0)
{
	assert( editorScene != nullptr );

	//fbo = new FBO( Settings(PICK_TARGET_SIZE, PICK_TARGET_SIZE) );
	//tex = fbo->createRenderTexture();
	//fbo->check();
	//fbo->unbind();

	//Viewframe* viewframe = editor->getMainViewframe();
	//CameraPtr camera = viewframe->getCamera();

	//view = fbo->addViewport( camera );
	//view->setClearColor(Colors::Black);
}

//-----------------------------------//

void GizmoMode::onModeInit(wxToolBar* toolBar, ModeIdMap& map)
{
	toolBar->AddSeparator();

	toolBar->AddTool( Gizmo_Camera, "Camera", wxMEMORY_BITMAP(camera), 
		"Selects the Camera View tool", wxITEM_RADIO );

	toolBar->AddTool( Gizmo_Select, "Select", wxMEMORY_BITMAP(cursor), 
		"Selects the Entity Selection tool", wxITEM_RADIO );

	toolBar->AddTool( Gizmo_Translate, "Move", wxMEMORY_BITMAP(move), 
		"Selects the Move tool", wxITEM_RADIO );

	toolBar->AddTool( Gizmo_Rotate, "Rotate", wxMEMORY_BITMAP(rotate2), 
		"Selects the Rotate tool", wxITEM_RADIO );

	toolBar->AddTool( Gizmo_Scale, "Scale", wxMEMORY_BITMAP(scale), 
		"Selects the Scale tool", wxITEM_RADIO );

	map[Gizmo_Camera] = this;
	map[Gizmo_Select] = this;
	map[Gizmo_Translate] = this;
	map[Gizmo_Rotate] = this;
	map[Gizmo_Scale] = this;
}

//-----------------------------------//

void GizmoMode::onModeEnter( int id )
{
	tool = id;
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
	tr->setDebugRenderableVisible( false );

	GizmoNodeMap::iterator it = gizmos.find(node);
	
	if( it == gizmos.end() )
		return;

	const NodePtr& gizNode = gizmos[node];
	
	editorScene->remove(gizNode);
	gizmos.erase(it);
}

//-----------------------------------//

void GizmoMode::drawGizmo( NodePtr old, NodePtr new_ )
{
	if( tool != Gizmo_Translate )
		return;

	if( old )
		disableBoundingGizmo( old );

	if( new_ && ( gizmos.find(new_) == gizmos.end() ) )
		enableBoundingGizmo( new_ );

	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::onNodeSelected( NodePtr old, NodePtr new_ )
{
	assert( new_ != nullptr );  

	switch(tool) 
	{	
		case Gizmo_Select:
		{
			TransformPtr transform = new_->getTransform();
			transform->setDebugRenderableVisible( true );
			break;
		}

		case Gizmo_Translate:
		case Gizmo_Rotate:
		case Gizmo_Scale:
		{
			drawGizmo( old, new_ );
			break;
		} 
	}

	selected.push_back( new_ );
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

	Color pick;
	glReadPixels(m_x, m_y, 1, 1, GL_RGB, GL_FLOAT, &pick);
	
	if( glHasError("Error reading pixels from framebuffer") )
		return false;

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

void GizmoMode::onMouseMove( const MouseMoveEvent& me )
{
	if( ((tool != Gizmo_Translate) 
		&& (tool != Gizmo_Rotate)
		&& (tool != Gizmo_Scale))
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

	if( !pickImageTest(me, axis) )
	{
		gizmo->deselectAxis();
		viewframe->flagRedraw();
		return;
	}

	gizmo->selectAxis(axis);
	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::onMouseDrag( const MouseDragEvent& de )
{
	if( !gizmo || !gizmo->isAnyAxisSelected() )
		return;

	Vector3 unit = Gizmo::getUnitVector(axis);

	if( axis == GizmoAxis::X )
		unit *= de.dx;
	else if( axis == GizmoAxis::Y )
		unit *= de.dy;
	else if( axis == GizmoAxis::Z )
		unit *= de.dx;
	
	const NodePtr& node = gizmo->getNodeAttachment();
	const TransformPtr& transform = node->getTransform();
	transform->translate( unit );
	
	const NodePtr& giz_node = gizmo->getNode();
	const TransformPtr& giz_tr = giz_node->getTransform();
	giz_tr->translate( unit );

	viewframe->flagRedraw();
}

//-----------------------------------//

void GizmoMode::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	const ScenePtr& scene = engine->getSceneManager();
	const CameraPtr& camera = viewframe->getCamera();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( mbe.x, mbe.y, &outFar );

#if 0 // Enable this to draw debugging lines
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

void GizmoMode::disableSelectedNodes()
{
	// Disable all enabled bounding boxes.
	foreach( const NodePtr& node, selected )
		disableBoundingGizmo( node );

	selected.clear();
	viewframe->flagRedraw();
}

//-----------------------------------//

NodePtr buildRay( const Ray& pickRay, const Vector3& outFar )
{
	std::vector< Vector3 > vertex;
	vertex.push_back( pickRay.origin );
	vertex.push_back( outFar );

	std::vector< Vector3 > colors;
	colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );
	colors.push_back( Vector3( 1.0f, 0.0f, 0.0f ) );

	VertexBufferPtr vb( new VertexBuffer() );
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::Color, colors );

	MaterialPtr mat( new Material("LineMaterial") );
	
	RenderablePtr rend( new Renderable( Primitive::Lines, vb ) );
	rend->setMaterial( mat );	
	
	GeometryPtr geom( new Geometry(rend) );
	
	NodePtr line( new Node( "line" ) );
	line->addTransform();
	line->addComponent( geom );
	line->setTag( Tags::NonPickable, true );
	line->setTag( EditorTags::EditorOnly, true );
	
	return line;
}

//-----------------------------------//

} } // end namespaces