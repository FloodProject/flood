/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "PCH.h"
#include "SceneTreeCtrl.h"
#include "ArtProvider.h"
#include "icons.h"

using namespace vapor;
using namespace vapor::scene;
using namespace vapor::render;

namespace vapor { namespace editor {

typedef std::pair< const std::string, ComponentPtr > componentPair;

//-----------------------------------//

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(SceneTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_SceneTree, SceneTreeCtrl::onItemMenu)
	EVT_TREE_SEL_CHANGED(ID_SceneTree, SceneTreeCtrl::onItemChanged)
	EVT_TREE_END_LABEL_EDIT(ID_SceneTree, SceneTreeCtrl::onLabelEdit)
	//EVT_SET_FOCUS(SceneTreeCtrl::onActivate)
	EVT_TREE_BEGIN_DRAG(ID_SceneTree, SceneTreeCtrl::onDragBegin)
	EVT_TREE_END_DRAG(ID_SceneTree, SceneTreeCtrl::onDragEnd)
	EVT_RIGHT_UP(SceneTreeCtrl::onMouseRightUp)
	EVT_MENU(wxID_ANY, SceneTreeCtrl::onNodeMenu)
END_EVENT_TABLE()

//-----------------------------------//

SceneTreeCtrl::SceneTreeCtrl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id,
					const wxPoint& pos,
					const wxSize& size,
					long style,
					const wxValidator& validator, 
					const wxString&	name)
	: wxTreeCtrl(parent, id, pos, size, style, validator, name),
		engine(engine), activated(false)
{
	if( !engine )
	{
		assert( "Invalid engine instance." );
	}

	scene = engine->getSceneManager();
	
	initIcons();
	initControl();

	updateScene( root, scene.lock() );
}

//-----------------------------------//

SceneTreeCtrl::~SceneTreeCtrl()
{

}

//-----------------------------------//

void SceneTreeCtrl::initControl()
{
	// Add the root node
	wxString str(scene.lock()->getName());
	root = AddRoot(str.Capitalize(), 1);

	ExpandAll();
	
	//updateScene();
	scene.lock()->onNodeAdded += fd::bind( &SceneTreeCtrl::onNodeAdded, this );
	scene.lock()->onNodeRemoved += fd::bind( &SceneTreeCtrl::onNodeRemoved, this );
}

//-----------------------------------//

void SceneTreeCtrl::initIcons()
{
	// create a new list of all the icons
	imageList = new wxImageList(16, 16, false, 10);

	// the images were preconverted from image files to binary data 
	// held in a regular C++ array. this way we don't need to package
	// external image files with the executable. but we do need to
	// convert the images from the array to an wxBitmap.

	imageList->Add(wxMEMORY_BITMAP(package));
	componentIcons["Scene"] = imageList->Add(wxMEMORY_BITMAP(sitemap_color));
	componentIcons["Camera"] = imageList->Add(wxMEMORY_BITMAP(camera));
	componentIcons["Light"] = imageList->Add(wxMEMORY_BITMAP(lightbulb_off));
	componentIcons["Sound"] = imageList->Add(wxMEMORY_BITMAP(sound));
	componentIcons["Listener"] = imageList->Add(wxMEMORY_BITMAP(status_online));
	componentIcons["Terrain"] = imageList->Add(wxMEMORY_BITMAP(world));
	componentIcons["Transform"] = imageList->Add(wxMEMORY_BITMAP(vector_icon));
	componentIcons["Grid"] = imageList->Add(wxMEMORY_BITMAP(grid_icon_white_bg));
	componentIcons["Geometry"] = imageList->Add(wxMEMORY_BITMAP(shape_flip_horizontal));
	componentIcons["Body"] = imageList->Add(wxMEMORY_BITMAP(link));

	AssignImageList(imageList);
}

//-----------------------------------//

void SceneTreeCtrl::updateScene( wxTreeItemId id, NodePtr node )
{
	// TODO: fix it

	GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );
	
	if( group )
	{
		// Traverse the tree and add the nodes to the control.
		foreach( NodePtr child, group->getChildren() )
		{
			wxTreeItemId new_id = AppendItem( id, child->getName(), 0 );
			updateScene( new_id, child );
		}
	}

	foreach( componentPair component, node->getComponents() )
	{
		const std::string& type = component.second->getType();
		AppendItem( id, type, componentIcons[type] );
	}
}


//-----------------------------------//

scene::NodePtr SceneTreeCtrl::getEntity( wxTreeItemId id )
{
	assert( id.IsOk() );

	std::string str( GetItemText( id ) );
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

void SceneTreeCtrl::setBoundingBox( const wxTreeItemId& id, bool state )
{
	NodePtr node = getEntity( id );

	if( !node ) return;

	// Activate bounding box drawing for this node.
	foreach( GeometryPtr geometry, node->getGeometry() )
		geometry->setBoundingBoxVisible( state );
}

//-----------------------------------//

void SceneTreeCtrl::onItemChanged(wxTreeEvent& event)
{
	// Turn off last selected tree item's bounding box.
	wxTreeItemId old = event.GetOldItem();
	if( old.IsOk() ) setBoundingBox( old, false );

	// Turn on the new tree item's bounding box.
	wxTreeItemId id = event.GetItem();
	setBoundingBox( id, true );
}

//-----------------------------------//

void SceneTreeCtrl::onItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	NodePtr node = getEntity( menuItemId );

#if wxUSE_MENUS
	wxMenu menu("Scene node");

	if( node )
	{
		menu.AppendCheckItem(ID_MenuSceneNodeVisible, "&Visible");
		menu.Check(ID_MenuSceneNodeVisible, node->getVisible() );

		const std::vector< GeometryPtr >& geo = node->getGeometry();
		if( !geo.empty() )
		{
			const std::vector< RenderablePtr >& rend = geo.front()->getRenderables();
			if( !rend.empty() )
			{
				bool state = (rend.front()->getRenderMode() != RenderMode::Solid);
				menu.AppendCheckItem(ID_MenuSceneNodeWireframe, "&Wireframe");
				menu.Check(ID_MenuSceneNodeWireframe, state );
			}
		}
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
		NodePtr node = getEntity( menuItemId );
		if( !node ) return;
		
		node->setVisible( !node->getVisible() );
	}

	if( event.GetId() == ID_MenuSceneNodeWireframe )
	{
		NodePtr node = getEntity( menuItemId );
		if( !node ) return;

		RenderMode::Enum mode = event.IsChecked()
			? RenderMode::Wireframe : RenderMode::Solid;

		foreach( GeometryPtr geo, node->getGeometry() )
		{
			foreach( RenderablePtr rend, geo->getRenderables() )
			{
				rend->setRenderMode( mode );
			}
		}
	}
}

//-----------------------------------//

void SceneTreeCtrl::onNodeAdded( const scene::GroupEvent& event )
{
	wxTreeItemId id = AppendItem( root, event.node->getName(), 0 );

	foreach( componentPair component, event.node->getComponents() )
	{
		const std::string& type = component.second->getType();
		AppendItem( id, type, componentIcons[type] );
	}
}

//-----------------------------------//

void SceneTreeCtrl::onNodeRemoved( const scene::GroupEvent& event )
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

void SceneTreeCtrl::onActivate( wxFocusEvent& event )
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

	NodePtr src = getEntity( dragItemId );
	NodePtr dest = getEntity( drag_id );

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