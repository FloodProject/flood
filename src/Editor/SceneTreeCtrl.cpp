/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "SceneTreeCtrl.h"
#include "ArtProvider.h"
#include "icons.h"

using namespace vapor;
using namespace vapor::scene;

namespace vapor { namespace editor {

typedef std::pair< const std::string, ComponentPtr > componentPair;

//-----------------------------------//

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(SceneTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_MENU(ID_SceneTree, SceneTreeCtrl::onItemMenu)
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
		engine(engine)
{
	if( !engine )
	{
		assert( "Invalid engine instance." );
	}

	scene = engine->getSceneManager();
	
	initIcons();
	InitControl();

	ExpandAll();
}

//-----------------------------------//

SceneTreeCtrl::~SceneTreeCtrl()
{

}

//-----------------------------------//

void SceneTreeCtrl::InitControl()
{
	// Add the root node
	wxString str(scene->getName());
	root = AddRoot(str.Capitalize(), 1);

	ExpandAll();
	
	//updateScene();
	scene->onNodeAdded += fd::bind( &SceneTreeCtrl::onNodeAdded, this );
	scene->onNodeRemoved += fd::bind( &SceneTreeCtrl::onNodeRemoved, this );
}

//-----------------------------------//

void SceneTreeCtrl::updateScene()
{
	// traverse the scene tree and add nodes
	foreach( NodePtr node, scene->getChildren() )
	{
		wxTreeItemId id = AppendItem( root, node->getName(), 0 );

		foreach( componentPair component, node->getComponents() )
		{
			const std::string& type = component.second->getType();
			AppendItem( id, type, componentIcons[type] );
		}
	}
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
	componentIcons["Grid"] = imageList->Add(wxMEMORY_BITMAP(grid_icon));

	AssignImageList(imageList);
}

//-----------------------------------//

void SceneTreeCtrl::onItemMenu(wxTreeEvent& event)
{
    wxTreeItemId itemId = event.GetItem();
	//debug( "%s", itemId.IsOk() ? "true" : "false" );
    //MyTreeItemData *item = itemId.IsOk() ? (MyTreeItemData *)GetItemData(itemId)
                                         //: NULL;
    wxPoint clientpt = event.GetPoint();

#if wxUSE_MENUS
    wxMenu menu("Scene node");
    menu.Append(ID_MenuSceneNodeDelete, "&Delete...");

    PopupMenu(&menu, clientpt);
#endif // wxUSE_MENUS

    event.Skip();
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

	ExpandAllChildren( id );
}

//-----------------------------------//

void SceneTreeCtrl::onNodeRemoved( const scene::GroupEvent& event )
{
	//const std::string& type = component.second->getType();
	//AppendItem( id, type, componentIcons[type] );
}

//-----------------------------------//

} } // end namespaces