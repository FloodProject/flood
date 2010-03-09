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

	switch(currentTool)
	{
	case Gizmo_Camera:

		break;
	case Gizmo_Select:

		break;
	case Gizmo_Translate:

		break;
	case Gizmo_Rotate:

		break;
	case Gizmo_Scale:

		break;
	default:
		warn( "editor", "Invalid tool id in gizmo mode" );
		break;
	}
}

//-----------------------------------//

void GizmoMode::onModeExit()
{
	disableSelectedNodes();
}

//-----------------------------------//

void GizmoMode::onMouseClick(short x, short y)
{
	const ScenePtr& scene = engine->getSceneManager();

	disableSelectedNodes();

	// Get a ray given the screen location clicked.
	Viewport* viewport = editor->viewport;
	const Ray& pickRay = viewport->camera->getRay( x, y );

	// Perform ray casting to find the nodes.
	RayBoxQueryList list;
	scene->doRayBoxQuery( pickRay, list );

	foreach( const RayBoxQueryResult& res, list )
	{
		const NodePtr& node = res.node;

		node->getTransform()->setDebugRenderableVisible( true );
		selectedNodes.push_back( node );
	}
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