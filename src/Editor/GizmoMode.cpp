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

namespace vapor { namespace editor {

enum
{
	Gizmo_Camera = 18237,
	Gizmo_Select,
	Gizmo_Translate,
	Gizmo_Rotate,
	Gizmo_Scale
};

//-----------------------------------//

GizmoMode::GizmoMode( EditorFrame* frame )
	: Mode( frame ), editorScene( frame->getEditorScene() )
{
	assert( editorScene != nullptr );
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
	currentTool = id;
}

//-----------------------------------//

void GizmoMode::onModeExit()
{
	disableSelectedNodes();
}

//-----------------------------------//

void GizmoMode::enableBoundingGizmo( const NodePtr& node )
{
	static int i = 0;
	assert( node != nullptr );

	const TransformPtr& tr = node->getTransform();
	tr->setDebugRenderableVisible( true );

	assert( gizmos.find(node) == gizmos.end() );

	std::string str( "Gizmo"+num_to_str(i) );
	NodePtr gizNode( new Node(str) );
	gizNode->addTransform();

	Vector3 center( tr->getBoundingVolume().getCenter() );
	GeometryPtr gizmo( new Gizmo(center ) ); 	
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
	if( currentTool != Gizmo_Translate ) return;

	if( old )
		disableBoundingGizmo( old );

	if( new_ && (gizmos.find(new_) == gizmos.end()) )
		enableBoundingGizmo( new_ );
}

//-----------------------------------//

void GizmoMode::onNodeSelected( NodePtr old, NodePtr new_ )
{
	if( !new_ ) return;

	switch(currentTool)
	{
	case Gizmo_Camera:
		break;
	case Gizmo_Select:
		new_->getTransform()->setDebugRenderableVisible( true );
		selectedNodes.push_back( new_ );
		break;
	case Gizmo_Translate:
	case Gizmo_Rotate:
	case Gizmo_Scale:
		drawGizmo( old, new_ );
		break;
	default:
		warn( "editor", "Invalid tool id in gizmo mode" );
		break;
	}
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

	MaterialPtr mat( new Material( "LineMaterial", "diffuse" ) );
	
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

void GizmoMode::onMouseMove( const MouseMoveEvent& me )
{
	if( (currentTool != Gizmo_Translate) 
		&& (currentTool != Gizmo_Rotate)
		&& (currentTool != Gizmo_Scale) ) return;

	const CameraPtr& camera = viewframe->getCamera();

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = camera->getRay( me.x, me.y, &outFar );

	// Perform ray casting to find the nodes.
	RayBoxQueryResult res;
	
	if( !editorScene->doRayBoxQuery(pickRay, res) )
		return;

	// Find out if we picked a gizmo...
	GizmoPtr gizmo = res.node->getComponent<Gizmo>("Gizmo");
	
	if( !gizmo )
		return;

	//gizmo->
}

//-----------------------------------//

void GizmoMode::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	disableSelectedNodes();

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
		onNodeSelected( NodePtr(), res.node );
}

//-----------------------------------//

void GizmoMode::disableSelectedNodes()
{
	// Disable all enabled bounding boxes.
	foreach( const NodePtr& node, selectedNodes )
		disableBoundingGizmo( node );

	selectedNodes.clear();
}

//-----------------------------------//

} } // end namespaces