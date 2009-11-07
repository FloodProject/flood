/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "SceneTreeCtrl.h"
#include "icons.h"

using namespace vapor;
using namespace vapor::scene;

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
	
	initControl();
	initIcons();

	ExpandAll();
}

//-----------------------------------//

SceneTreeCtrl::~SceneTreeCtrl()
{

}

//-----------------------------------//

void SceneTreeCtrl::initControl()
{
	// Add the root node
	wxString str(scene->getName());
	wxTreeItemId root = AddRoot(str.Capitalize(), 0);
	
	// traverse the scene tree and add nodes
	//Node* node = scene->children;
	AppendItem(root, "Camera", 1);
	AppendItem(root, "Light", 2);
	AppendItem(root, "Sound", 3);
	AppendItem(root, "Listener", 4);
	AppendItem(root, "Terrain", 5);
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
	//imageList->Add(wxMEMORY_BITMAP(sitemap_color));
	//imageList->Add(wxMEMORY_BITMAP(camera));
	//imageList->Add(wxMEMORY_BITMAP(lightbulb_off));
	//imageList->Add(wxMEMORY_BITMAP(sound));
	//imageList->Add(wxMEMORY_BITMAP(status_online));
	//imageList->Add(wxMEMORY_BITMAP(world));

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