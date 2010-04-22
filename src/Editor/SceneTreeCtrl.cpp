/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "SceneTreeCtrl.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(SceneTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_SceneTree, SceneTreeCtrl::onItemMenu)
	EVT_TREE_SEL_CHANGED(ID_SceneTree, SceneTreeCtrl::onItemChanged)
	EVT_TREE_END_LABEL_EDIT(ID_SceneTree, SceneTreeCtrl::onLabelEdit)
	EVT_TREE_BEGIN_DRAG(ID_SceneTree, SceneTreeCtrl::onDragBegin)
	EVT_TREE_END_DRAG(ID_SceneTree, SceneTreeCtrl::onDragEnd)
	EVT_RIGHT_UP(SceneTreeCtrl::onMouseRightUp)
	EVT_MENU(wxID_ANY, SceneTreeCtrl::onNodeMenu)
END_EVENT_TABLE()

//-----------------------------------//

SceneTreeCtrl::SceneTreeCtrl( vapor::Engine* engine, wxWindow* parent, wxWindowID id,
	const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, 
	const wxString&	name )
	: wxTreeCtrl(parent, id, pos, size, style, validator, name),
	engine(engine), activated(false)
{
	assert( engine != nullptr );

	scene = engine->getSceneManager();
	
	initIcons();
	initControl();

	updateScene( root, scene.lock() );
}

//-----------------------------------//

void SceneTreeCtrl::initControl()
{
	// Add the root node.
	wxString str( scene.lock()->getName() );
	root = AddRoot(str.Capitalize(), 1);

	ExpandAll();

	scene.lock()->onNodeAdded += fd::bind( &SceneTreeCtrl::onNodeAdded, this );
	scene.lock()->onNodeRemoved += fd::bind( &SceneTreeCtrl::onNodeRemoved, this );
}

//-----------------------------------//

void SceneTreeCtrl::initIcons()
{
	// create a new list of all the icons.
	imageList = new wxImageList(16, 16, false, 12);

	// the images were preconverted from image files to binary data 
	// held in a regular C++ array. this way we don't need to package
	// external image files with the executable. but we do need to
	// convert the images from the array to an wxBitmap.

	imageList->Add(wxMEMORY_BITMAP(package));
	componentIcons["Scene"] = imageList->Add(wxMEMORY_BITMAP(sitemap_color));
	componentIcons["Camera"] = imageList->Add(wxMEMORY_BITMAP(camera));
	componentIcons["FirstPersonCamera"] = imageList->Add(wxMEMORY_BITMAP(camera));
	componentIcons["Light"] = imageList->Add(wxMEMORY_BITMAP(lightbulb_off));
	componentIcons["Sound"] = imageList->Add(wxMEMORY_BITMAP(sound));
	componentIcons["Listener"] = imageList->Add(wxMEMORY_BITMAP(status_online));
	componentIcons["Terrain"] = imageList->Add(wxMEMORY_BITMAP(world));
	componentIcons["Transform"] = imageList->Add(wxMEMORY_BITMAP(chart_line));
	componentIcons["Grid"] = imageList->Add(wxMEMORY_BITMAP(grid_icon_white_bg));
	componentIcons["Geometry"] = imageList->Add(wxMEMORY_BITMAP(shape_flip_horizontal));
	componentIcons["Body"] = imageList->Add(wxMEMORY_BITMAP(link));
	componentIcons["Skydome"] = imageList->Add(wxMEMORY_BITMAP(skydome));
	componentIcons["Gizmo"] = imageList->Add(wxMEMORY_BITMAP(vector_icon));

	AssignImageList(imageList);
}

//-----------------------------------//

void SceneTreeCtrl::updateScene( wxTreeItemId id, const NodePtr& node )
{
	// TODO: fix it

	GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );
	
	if( group )
	{
		// Traverse the tree and add the nodes to the control.
		foreach( const NodePtr& child, group->getChildren() )
		{
			if( child->getTag( EditorTags::EditorOnly ) )
				continue;

			wxTreeItemId new_id = AppendItem( id, child->getName(), 0 );
			updateScene( new_id, child );
		}
	}

	foreach( const ComponentMapPair& component, node->getComponents() )
	{
		const std::string& type = component.second->getType();
		AppendItem( id, type, componentIcons[type] );
	}
}


//-----------------------------------//

scene::NodePtr SceneTreeCtrl::getEntity( wxTreeItemId id )
{
	if( !id ) return NodePtr();

	const std::string str( GetItemText( id ) );
	NodePtr node = scene.lock()->getEntity( str );

	while( !node )
	{
		// Search for a parent tree item.
		id = GetItemParent( id );

		// If there is none, abort.
		if( !id.IsOk() ) return NodePtr();
		
		node = scene.lock()->getEntity( str );
	}

	return node;
}

//-----------------------------------//

void SceneTreeCtrl::onItemChanged(wxTreeEvent& event)
{
	// Turn off last selected tree item's bounding box.
	wxTreeItemId old = event.GetOldItem();
	wxTreeItemId id = event.GetItem();

	//if( old.IsOk() ) 
	//	setBoundingBox( old, false );

	if( !onItemSelected.empty() )
		onItemSelected( old, id );

	// Turn on the new tree item's bounding box.
	//setBoundingBox( id, true );
}

//-----------------------------------//

void SceneTreeCtrl::onItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	const NodePtr& node = getEntity( menuItemId );

#if wxUSE_MENUS
	wxMenu menu("Scene node");

	if( node )
	{
		menu.AppendCheckItem(ID_MenuSceneNodeVisible, "&Visible");
		menu.Check(ID_MenuSceneNodeVisible, node->isVisible() );

		const std::vector< GeometryPtr >& geo = node->getGeometry();
		if( !geo.empty() )
		{
			const std::vector< RenderablePtr >& rend = geo.front()->getRenderables();
			if( !rend.empty() )
			{
				bool state = (rend.front()->getPolygonMode() != PolygonMode::Solid);
				menu.AppendCheckItem(ID_MenuSceneNodeWireframe, "&Wireframe");
				menu.Check(ID_MenuSceneNodeWireframe, state );
			}
		}
	}
	else
	{
		menu.AppendSeparator();

		menu.Append(ID_MenuSceneNodeAddMesh, "&Add Mesh");
		//menu.Append(ID_MenuSceneNodeAddLight, "&Add Light");
		//menu.Append(ID_MenuSceneNodeAddCamera, "&Add Camera");
		//menu.Append(ID_MenuSceneNodeAddSource, "&Add Audio Source");
		//menu.Append(ID_MenuSceneNodeAddTransform, "&Add Transform");
		//menu.Append(ID_MenuSceneNodeAddTrigger, "&Add Trigger");
		//menu.Append(ID_MenuSceneNodeAddSkybox, "&Add Skybox");
	}

	menu.Append(ID_MenuSceneNodeDelete, "&Delete...");

	wxPoint clientpt = event.GetPoint();
	PopupMenu(&menu, clientpt);
#endif // wxUSE_MENUS
}

//-----------------------------------//

void SceneTreeCtrl::onNodeMenu( wxCommandEvent& event )
{
	if( event.GetId() == ID_MenuSceneNodeVisible )
	{
		const NodePtr& node = getEntity( menuItemId );
		if( !node ) return;
		
		node->setVisible( !node->isVisible() );
	}

	if( event.GetId() == ID_MenuSceneNodeWireframe )
	{
		const NodePtr& node = getEntity( menuItemId );
		if( !node ) return;

		PolygonMode::Enum mode = event.IsChecked() ?
			PolygonMode::Wireframe : PolygonMode::Solid;

		foreach( const GeometryPtr& geo, node->getGeometry() )
		{
			foreach( const RenderablePtr& rend, geo->getRenderables() )
			{
				rend->setPolygonMode( mode );
			}
		}
	}

	if( event.GetId() == ID_MenuSceneNodeAddMesh )
	{
		//wxFileDialog fd( this, wxFileSelectorPromptStr,
		//	wxEmptyString, wxEmptyString, "Mesh files (*.ms3d)|*.ms3d",
		//	wxFD_DEFAULT_STYLE|wxFD_FILE_MUST_EXIST, wxPoint( 0, 0 ) );

		//if( fd.ShowModal() == wxID_OK )
		//{
		//	wxString filename = fd.GetFilename();
		//	const NodePtr& node = getEntity( menuItemId );
		//}
	}
}

//-----------------------------------//

void SceneTreeCtrl::onNodeAdded( const scene::GroupEvent& event )
{
	if( event.node->getTag( EditorTags::EditorOnly ) )
		return;

	wxTreeItemId id = AppendItem( root, event.node->getName(), 0 );

	foreach( const ComponentMapPair& component, event.node->getComponents() )
	{
		const std::string& type = component.second->getType();
		AppendItem( id, type, componentIcons[type] );
	}
}

//-----------------------------------//

void SceneTreeCtrl::onNodeRemoved( const scene::GroupEvent& /*event*/ )
{
	//const std::string& type = component.second->getType();
	//AppendItem( id, type, componentIcons[type] );
}

//-----------------------------------//

void SceneTreeCtrl::onLabelEdit( wxTreeEvent& event )
{
	if( event.IsEditCancelled() ) return;

	event.GetLabel();
}

//-----------------------------------//

void SceneTreeCtrl::onActivate( wxFocusEvent& /*event*/ )
{
	if( !activated )
	{
		ExpandAll();
		activated = true;
	}
}

//-----------------------------------//

void SceneTreeCtrl::onDragBegin( wxTreeEvent& event )
{
	//event.Allow();

	// Save the dragged original item, for later processing...
	dragItemId = event.GetItem();
}

//-----------------------------------//

void SceneTreeCtrl::onDragEnd( wxTreeEvent& event )
{
	// Get the drop point location and do an hit test to check if
	// it was was done in a valid tree item id location.
	
	wxPoint drag_point = event.GetPoint();
	wxTreeItemId drag_id = HitTest( drag_point );

	// If the drop was not done in a valid tree location, 
	// then we've got nothing to do here, move along...
	if( !drag_id.IsOk() ) return;

	const NodePtr& src = getEntity( dragItemId );
	const NodePtr& dest = getEntity( drag_id );

	if( src && dest ) // It is a node, just re-parent it.
	{
		src->setParent( dest );
	}
	else // It was probably a component, so handle it.
	{
		// TODO: move component from one node to another
	}

	//DeleteChildren( root );
	//updateScene( root, scene );
}

//-----------------------------------//

void SceneTreeCtrl::onMouseRightUp( wxMouseEvent& event )
{
	wxPoint clientpt = event.GetPosition();

#if wxUSE_MENUS
	wxMenu menu("Scene");
	//menu.Append(ID_MenuSceneNodeDelete, "&Delete...");

	PopupMenu(&menu, clientpt);
#endif // wxUSE_MENUS
}

//-----------------------------------//

} } // end namespaces