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
#include "Gizmo.h"
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
	: Mode( frame )
{
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

void GizmoMode::drawGizmo( NodePtr old, NodePtr new_ )
{
	if( currentTool != Gizmo_Translate ) return;

	if( old )
	{
		const TransformPtr& tr = old->getTransform();
		tr->setDebugRenderableVisible( false );
		old->removeComponent( "Gizmo" );
	}

	if( !new_ ) return;

	const TransformPtr& tr = new_->getTransform();
	tr->setDebugRenderableVisible( true );
	GeometryPtr gizmo( new Gizmo( tr->getBoundingVolume().getCenter() ) ); 	
	new_->addComponent(gizmo);
}

//-----------------------------------//

void GizmoMode::onNodeSelected( NodePtr old, NodePtr new_ )
{
	disableSelectedNodes();

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
	line->addComponent( TransformPtr( new Transform() ) );
	line->addComponent( geom );
	line->setTag( Tags::NonPickable, true );
	line->setTag( EditorTags::EditorOnly, true );
	
	return line;
}

//-----------------------------------//

void GizmoMode::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	const ScenePtr& scene = engine->getSceneManager();
	Viewport* const viewport = editor->viewport;

	// Get a ray given the screen location clicked.
	Vector3 outFar;
	const Ray& pickRay = viewport->camera->getRay( mbe.x, mbe.y, &outFar );

#if 1 // Enable this to draw debugging lines
	const NodePtr& line = buildRay( pickRay, outFar );
	editor->editorScene->add( line );
#endif

	// Perform ray casting to find the nodes.
	RayBoxQueryResult res;
	scene->doRayBoxQuery( pickRay, res );

	onNodeSelected( NodePtr(), res.node );
}

//-----------------------------------//

void GizmoMode::disableSelectedNodes()
{
	// Disable all enabled bounding boxes.
	foreach( const NodePtr& node, selectedNodes )
	{
		node->getTransform()->setDebugRenderableVisible( false );
	}

	selectedNodes.clear();
}

//-----------------------------------//

} } // end namespaces