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
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

enum 
{
	ID_SceneTree = 9832,
	ID_MenuSceneNodeDelete = wxID_DELETE,
	ID_MenuSceneNodeVisible = 5643,
	ID_MenuSceneNodeWireframe,
	ID_ButtonNodeAdd,
	ID_ButtonNodeDelete,
};

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(SceneTreeCtrl, wxPanel)
	EVT_TREE_ITEM_MENU(ID_SceneTree, SceneTreeCtrl::onItemMenu)
	EVT_TREE_SEL_CHANGED(ID_SceneTree, SceneTreeCtrl::onItemChanged)
	EVT_TREE_BEGIN_LABEL_EDIT(ID_SceneTree, SceneTreeCtrl::onLabelEditBegin)
	EVT_TREE_END_LABEL_EDIT(ID_SceneTree, SceneTreeCtrl::onLabelEditEnd)
	EVT_TREE_BEGIN_DRAG(ID_SceneTree, SceneTreeCtrl::onDragBegin)
	EVT_TREE_END_DRAG(ID_SceneTree, SceneTreeCtrl::onDragEnd)
	EVT_CONTEXT_MENU(SceneTreeCtrl::onMouseRightUp)
	EVT_MENU(wxID_ANY, SceneTreeCtrl::onNodeMenu)
	EVT_BUTTON(ID_ButtonNodeAdd, SceneTreeCtrl::onButtonNodeAdd)
	EVT_BUTTON(ID_ButtonNodeDelete, SceneTreeCtrl::onButtonNodeDelete)
END_EVENT_TABLE()

//-----------------------------------//

enum 
{
	ID_MenuSceneNodeComponentRangeStart = 9876,
	ID_MenuSceneNodeTransform,
	ID_MenuSceneNodeMesh,
	ID_MenuSceneNodeCamera,
	ID_MenuSceneNodeFirstPersonCamera,
	ID_MenuSceneNodeComponentRangeEnd
};

struct ComponentEntry
{
	bool show;
	int id;
	const char* name;
	const unsigned char* bmp;
	const int bmp_len;
};

#define BMP(s) s, sizeof(s)

static ComponentEntry components[] = {
	{ true, ID_MenuSceneNodeTransform, "Transform", BMP(chart_line) },
	{ true, ID_MenuSceneNodeMesh, "Model", BMP(shape_flip_horizontal) },
	{ true, ID_MenuSceneNodeCamera, "Camera", BMP(camera) },
	{ true, ID_MenuSceneNodeFirstPersonCamera, "FirstPersonCamera", BMP(camera) },
	{ true, wxID_ANY, "Light", BMP(lightbulb_off) },
	{ true, wxID_ANY, "Sound", BMP(sound) },
	{ true, wxID_ANY, "Listener", BMP(status_online) },
	{ true, wxID_ANY, "Terrain", BMP(world) },
	{ true, wxID_ANY, "Grid", BMP(grid_icon_white_bg) },
	{ true, wxID_ANY, "Geometry", BMP(shape_flip_horizontal) },
	{ true, wxID_ANY, "Body", BMP(link) },
	{ true, wxID_ANY, "Skydome", BMP(skydome) },
	{ true, wxID_ANY, "Gizmo", BMP(vector_icon) },
	{ false, wxID_ANY, "Scene", BMP(sitemap_color) }
};

std::map<const char*, wxBitmap> bitmaps;

//-----------------------------------//

class wxNodeItemData : public wxTreeItemData
{
public:

	NodeWeakPtr node;
	ComponentWeakPtr component;
};

//-----------------------------------//

SceneTreeCtrl::SceneTreeCtrl( EditorFrame* frame, Engine* engine,
							 wxWindow* parent, wxWindowID id )
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(230, -1) ),
	engine(engine), editor(frame), activated(false)
{
	assert( engine != nullptr );
	
	const ScenePtr& scene = engine->getSceneManager();
	weakScene = scene;

	initControl();
	initIcons();
	initScene( root, scene );
}

//-----------------------------------//

void SceneTreeCtrl::initControl()
{
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	tree = new wxTreeCtrl(this, ID_SceneTree, wxDefaultPosition,
		wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS 
		| wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT );

	sizer->Add( tree, 1, wxEXPAND, 0 );

	// Add the root node.
	ScenePtr scene( weakScene );
	wxString str( scene->getName() );
	root = tree->AddRoot(str.Capitalize(), 1);

	tree->ExpandAll();

	scene->onNodeAdded += fd::bind( &SceneTreeCtrl::onNodeAdded, this );
	scene->onNodeRemoved += fd::bind( &SceneTreeCtrl::onNodeRemoved, this );
	
	wxBoxSizer* sizer2 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBitmap* m_bitmap4 = new wxStaticBitmap( this, wxID_ANY,
		wxMEMORY_BITMAP(find) );
	sizer2->Add( m_bitmap4, 0, wxALL|wxEXPAND, 5 );
	
	wxTextCtrl* m_textCtrl13 = new wxTextCtrl( this, wxID_ANY );
	sizer2->Add( m_textCtrl13, 1, wxEXPAND|wxALL, 5 );
	
	buttonNodeDelete = new wxBitmapButton( this, ID_ButtonNodeDelete,
		wxMEMORY_BITMAP(package_delete) );
	buttonNodeDelete->SetBitmapDisabled( wxMEMORY_BITMAP(package_delete_disable) );
	buttonNodeDelete->Disable();
	sizer2->Add( buttonNodeDelete, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBitmapButton* m_bpButton51 = new wxBitmapButton( this, ID_ButtonNodeAdd,
		wxMEMORY_BITMAP(package_add) );
	sizer2->Add( m_bpButton51, 0, wxTOP|wxBOTTOM, 5 );

	sizer->Add( sizer2, 0, wxEXPAND, 0 );
	
	SetSizer( sizer );
}

//-----------------------------------//

void SceneTreeCtrl::initIcons()
{
	// create a new list of all the icons.
	imageList = new wxImageList(16, 16, false, 12);

	// the images were preconverted from image files to binary data 
	// held in a regular C++ array. this way we don't need to package
	// external image files with the executable. but we do need to
	// convert the images from the array to a wxBitmap.

	foreach( ComponentEntry& c, components )
	{
		bitmaps[c.name] = _wxConvertMemoryToBitmap(c.bmp, c.bmp_len);
	}

	imageList->Add(wxMEMORY_BITMAP(package));
	
	foreach( ComponentEntry& c, components )
	{
		icons[c.name] = imageList->Add(bitmaps[c.name]);
	}

	tree->AssignImageList(imageList);
}

//-----------------------------------//

void SceneTreeCtrl::initScene( wxTreeItemId id, const NodePtr& node )
{
	GroupPtr group( std::dynamic_pointer_cast<Group>(node) );
	
	if( !group )
	{
		addNode( node );
		return;
	}

	// Traverse the tree and add the nodes to the control.
	foreach( const NodePtr& child, group->getChildren() )
	{
		if( child->getTag(EditorTags::EditorOnly) )
			continue;

		initScene( id, child );
	}
}

//-----------------------------------//

NodePtr SceneTreeCtrl::getEntity( wxTreeItemId id )
{
	if( !id || !id.IsOk() )
		return NodePtr();

	wxNodeItemData* data = nullptr;
	data = (wxNodeItemData*) tree->GetItemData(id);
	
	assert( data != nullptr );

	return data->node.lock();
}

//-----------------------------------//

void SceneTreeCtrl::onItemChanged(wxTreeEvent& event)
{
	wxTreeItemId oldId = event.GetOldItem();
	wxTreeItemId newId = event.GetItem();

	const NodePtr& newNode = getEntity( newId );
	
	if( newNode )
		buttonNodeDelete->Enable();
	else
		buttonNodeDelete->Disable();

	if( !onItemSelected.empty() )
	{
		onItemSelected( oldId, newId );
	}
}

//-----------------------------------//

void SceneTreeCtrl::onItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	const NodePtr& node = getEntity( menuItemId );

	wxMenu menu("Scene node");

	if( !node )
		return;

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

	menu.Append(ID_MenuSceneNodeDelete, "&Delete");

	menu.AppendSeparator();

	foreach( ComponentEntry& c, components )
	{
		wxMenuItem* item = menu.Append(c.id, c.name);
		item->SetBitmap( bitmaps[c.name], false );
	}

	wxPoint clientpt = event.GetPoint();
	PopupMenu(&menu, clientpt);
}

//-----------------------------------//

void SceneTreeCtrl::onButtonNodeAdd(wxCommandEvent&)
{
	static int i = 0;
	std::string name( "SceneNode"+String::fromNumber(i++) );
	NodePtr node( new Node(name) );
	node->addTransform();
	
	// This is to prevent the editor of processing the node
	// in the method 'onNodeAdded', which could result in the
	// duplication of the node in the editor's scene tree.
	node->setTag( EditorTags::EditorOnly, true );
	
	ScenePtr scene( weakScene );
	scene->add(node);

	node->setTag( EditorTags::EditorOnly, false );

	wxTreeItemId id = addNode(node);

	tree->Expand( id );
	tree->SelectItem( id );
	tree->EditLabel( id );
}

//-----------------------------------//

void SceneTreeCtrl::onButtonNodeDelete(wxCommandEvent&)
{	
	wxTreeItemId id = tree->GetSelection();
	NodePtr node = getEntity(id);
	assert( node != nullptr );

	tree->Delete(id);
	weakScene.lock()->remove( node );

	editor->RefreshViewport();
}

//-----------------------------------//

void SceneTreeCtrl::onNodeMenu( wxCommandEvent& event )
{
	int id = event.GetId();

	if( id == ID_MenuSceneNodeVisible )
	{
		const NodePtr& node = getEntity( menuItemId );
		
		if( !node )
			return;
		
		node->setVisible( !node->isVisible() );
	}

	if( id == ID_MenuSceneNodeWireframe )
	{
		const NodePtr& node = getEntity( menuItemId );
		
		if( !node )
			return;

		PolygonMode::Enum mode = event.IsChecked() ?
			PolygonMode::Wireframe : PolygonMode::Solid;

		foreach( const GeometryPtr& geo, node->getGeometry() )
		{
			foreach( const RenderablePtr& rend, geo->getRenderables() )
			{
				rend->setPolygonMode( mode );
			}
		}

		Viewframe* viewframe = editor->getMainViewframe();
		viewframe->flagRedraw();
	}

	if( id > ID_MenuSceneNodeComponentRangeStart
		&& id < ID_MenuSceneNodeComponentRangeEnd )
	{
		onComponentAdd( event );
	}
}

//-----------------------------------//

void SceneTreeCtrl::onComponentAdd(wxCommandEvent& event )
{
	int id = event.GetId();

	if( id == ID_MenuSceneNodeMesh )
	{
		wxFileDialog fd( this, wxFileSelectorPromptStr,
			wxEmptyString, wxEmptyString, "Mesh files (*.ms3d)|*.ms3d",
			wxFD_DEFAULT_STYLE|wxFD_FILE_MUST_EXIST, wxPoint( 0, 0 ) );

		if( fd.ShowModal() == wxID_OK )
		{
			ResourceManager* rm = engine->getResourceManager();
			
			//FileSystem* fs = engine->getFileSystem();
			//fs->

			std::string filename( fd.GetPath() );
			MeshPtr mesh = rm->loadResource<Mesh>(filename);

			if( !mesh )
				return;

			ModelPtr model( new Model(mesh) );

			const NodePtr& node = getEntity( menuItemId );
			node->addComponent( model );
			addComponent( menuItemId, model );
		}
	}

	if( id == ID_MenuSceneNodeTransform )
	{
		const NodePtr& node = getEntity( menuItemId );
		node->addTransform();
		addComponent( menuItemId, node->getTransform() );
	}
}

//-----------------------------------//

void SceneTreeCtrl::addComponent( wxTreeItemId id, ComponentPtr comp )
{
	assert( comp != nullptr );
	const std::string& type = comp->getType();

	wxTreeItemId ch = tree->AppendItem( id, type, icons[type] );

	wxNodeItemData* data = new wxNodeItemData();
	data->component = comp;

	tree->SetItemData( ch, data );

	editor->RefreshViewport();
}

//-----------------------------------//

wxTreeItemId SceneTreeCtrl::addNode( const NodePtr& node )
{
	assert( !node->getName().empty() );

	wxTreeItemId id = tree->AppendItem( root, node->getName(), 0 );

	wxNodeItemData* data = new wxNodeItemData();
	data->node = node;

	tree->SetItemData( id, data );

	foreach( const ComponentMapPair& component, node->getComponents() )
	{
		addComponent(id, component.second);
	}

	return id;
}

//-----------------------------------//

void SceneTreeCtrl::onNodeAdded( const GroupEvent& event )
{
	if( event.node->getTag(EditorTags::EditorOnly) )
		return;
	
	addNode(event.node);
}

//-----------------------------------//

void SceneTreeCtrl::onNodeRemoved( const GroupEvent& /*event*/ )
{
	//const std::string& type = component.second->getType();
	//AppendItem( id, type, icons[type] );
}

//-----------------------------------//

void SceneTreeCtrl::onLabelEditBegin( wxTreeEvent& event )
{
	wxTreeItemId item = event.GetItem();
	NodePtr node = getEntity( item );
	
	if( !node )
	{
		event.Veto();
	}
}

//-----------------------------------//

void SceneTreeCtrl::onLabelEditEnd( wxTreeEvent& event )
{
	ScenePtr scene( weakScene );
	wxTreeItemId item = event.GetItem();
	const wxString& label = event.GetLabel();

	if( event.IsEditCancelled() )
	{
		event.Veto();
		return;
	}

	// TODO: test for duplicate scene names
	if( label.empty() )
	{
		event.Veto();
		return;
	}

	NodePtr node = getEntity( item );
	assert( node != nullptr );

	node->setName( std::string( label.c_str() ) );
}

//-----------------------------------//

void SceneTreeCtrl::onActivate( wxFocusEvent& /*event*/ )
{
	if( !activated )
	{
		tree->ExpandAll();
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
	wxTreeItemId drag_id = tree->HitTest( drag_point );

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

void SceneTreeCtrl::onMouseRightUp( wxContextMenuEvent& event )
{
	wxPoint clientpt = event.GetPosition();

	wxMenu menu("Scene");
	//menu.Append(ID_MenuSceneNodeDelete, "&Delete...");

	PopupMenu(&menu, clientpt);
}

//-----------------------------------//

} } // end namespaces