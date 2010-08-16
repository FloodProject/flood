/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ScenePage.h"
#include "Events.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Viewframe.h"

namespace vapor { namespace editor {

//-----------------------------------//

enum 
{
	ID_MenuSceneNodeDelete = wxID_DELETE,
	ID_SceneTree = wxID_HIGHEST+8321,
	ID_MenuSceneNodeVisible,
	ID_MenuSceneNodeWireframe,
	ID_ButtonNodeAdd,
	ID_ButtonNodeDelete,
};

BEGIN_EVENT_TABLE(ScenePage, wxPanel)
	EVT_TREE_ITEM_MENU(ID_SceneTree, ScenePage::onItemMenu)
	EVT_TREE_SEL_CHANGED(ID_SceneTree, ScenePage::onItemChanged)
	EVT_TREE_BEGIN_LABEL_EDIT(ID_SceneTree, ScenePage::onLabelEditBegin)
	EVT_TREE_END_LABEL_EDIT(ID_SceneTree, ScenePage::onLabelEditEnd)
	EVT_TREE_BEGIN_DRAG(ID_SceneTree, ScenePage::onDragBegin)
	EVT_TREE_END_DRAG(ID_SceneTree, ScenePage::onDragEnd)
	EVT_CONTEXT_MENU(ScenePage::onMouseRightUp)
	EVT_MENU(wxID_ANY, ScenePage::onNodeMenu)
	EVT_BUTTON(ID_ButtonNodeAdd, ScenePage::onButtonNodeAdd)
	EVT_BUTTON(ID_ButtonNodeDelete, ScenePage::onButtonNodeDelete)
	EVT_UPDATE_UI(ID_ButtonNodeDelete, ScenePage::onButtonNodeDeleteUpdate)
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

static std::map<const char*, wxBitmap> bitmaps;

//-----------------------------------//

ScenePage::ScenePage( EditorFrame* frame, wxWindow* parent, wxWindowID id )
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(230, -1) )
	, editor(frame)
	, treeCtrl(nullptr)
	, imageList(nullptr)
	, buttonNodeAdd(nullptr)
	, buttonNodeDelete(nullptr)
	, sentLastSelectionEvent(false)
{
	engine = editor->getEngine();
	
	const ScenePtr& scene = engine->getSceneManager();
	weakScene = scene;

	initControl();
	initIcons();
	
	// Populates the control with the contents of the scene.
	addNodeRecursively( rootId, scene );
}

//-----------------------------------//

ScenePage::~ScenePage()
{
	//ScenePtr scene( weakScene );
	//scene->onNodeAdded -= fd::bind( &ScenePage::onNodeAdded, this );
	//scene->onNodeRemoved -= fd::bind( &ScenePage::onNodeRemoved, this );
}

//-----------------------------------//

void ScenePage::initControl()
{
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	treeCtrl = new wxTreeCtrl(this, ID_SceneTree, wxDefaultPosition,
		wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS 
		| wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT );

	sizer->Add( treeCtrl, 1, wxEXPAND, 0 );

	// Add the root node.
	ScenePtr scene( weakScene );
	wxString str( scene->getName() );
	rootId = treeCtrl->AddRoot(str.Capitalize(), 1);

	scene->onNodeAdded += fd::bind( &ScenePage::onNodeAdded, this );
	scene->onNodeRemoved += fd::bind( &ScenePage::onNodeRemoved, this );
	
	wxBoxSizer* sizer2 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBitmap* iconFind = new wxStaticBitmap( this, wxID_ANY,
		wxMEMORY_BITMAP(find) );
	sizer2->Add( iconFind, 0, wxALL|wxEXPAND, 5 );
	
	wxTextCtrl* textFind = new wxTextCtrl( this, wxID_ANY );
	sizer2->Add( textFind, 1, wxEXPAND|wxALL, 5 );
	
	// Node delete button.
	buttonNodeDelete = new wxBitmapButton( this, ID_ButtonNodeDelete,
		wxMEMORY_BITMAP(package_delete) );
	wxBitmap iconDisabled = wxMEMORY_BITMAP(package_delete_disable);
	buttonNodeDelete->SetBitmapDisabled( iconDisabled );
	buttonNodeDelete->Disable();
	sizer2->Add( buttonNodeDelete, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	// Node add button.
	buttonNodeAdd = new wxBitmapButton( this, ID_ButtonNodeAdd,
		wxMEMORY_BITMAP(package_add) );
	sizer2->Add( buttonNodeAdd, 0, wxTOP|wxBOTTOM, 5 );

	sizer->Add( sizer2, 0, wxEXPAND, 0 );
	SetSizer( sizer );
}

//-----------------------------------//

void ScenePage::initIcons()
{
	// Create a new list of all the icons.
	imageList = new wxImageList(16, 16, false, 12);

	// The images were preconverted from image files to binary data 
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

	treeCtrl->AssignImageList(imageList);
}

//-----------------------------------//

void ScenePage::addNodeRecursively( wxTreeItemId id, const NodePtr& node )
{
	const Class& nodeType = node->getInstanceType();
	
	if( nodeType.inherits<Group>() )
	{
		GroupPtr group = std::dynamic_pointer_cast<Group>(node);

		foreach( const NodePtr& child, group->getChildren() )
		{
			addNodeRecursively( id, child );
		}
	}
	else
	{
		addNode( node );
	}
}

//-----------------------------------//

wxTreeItemId ScenePage::addNode( const NodePtr& node )
{
	assert( !node->getName().empty() );

	wxTreeItemId id = treeCtrl->AppendItem( rootId, node->getName(), 0 );

	NodeItemData* data = new NodeItemData();
	data->node = node;

	treeCtrl->SetItemData( id, data );

	nodeIds[node] = id;

	foreach( const ComponentMapPair& component, node->getComponents() )
	{
		addComponent(id, component.second);
	}

	return id;
}

//-----------------------------------//

void ScenePage::addComponent( wxTreeItemId id, ComponentPtr component )
{
	assert( component != nullptr );
	const std::string& type = component->getInstanceType().getName();

	wxTreeItemId ch = treeCtrl->AppendItem( id, type, icons[type] );

	NodeItemData* data = new NodeItemData();
	data->component = component;

	treeCtrl->SetItemData( ch, data );

	editor->RefreshViewport();
}

//-----------------------------------//

NodePtr ScenePage::getNodeFromTreeId( wxTreeItemId id )
{
	if( !id || !id.IsOk() )
		return NodePtr();

	NodeItemData* data = (NodeItemData*) treeCtrl->GetItemData(id);
	assert( data != nullptr );

	return data->node.lock();
}

//-----------------------------------//

wxTreeItemId ScenePage::getTreeIdFromNode(const NodePtr& node)  
{
	NodeIdsMap::iterator it = nodeIds.find(node);

	if( it == nodeIds.end() )
		return wxTreeItemId();

	return nodeIds[node];
}

//-----------------------------------//

void ScenePage::onItemChanged(wxTreeEvent& event)
{
	wxTreeItemId oldId = event.GetOldItem();
	wxTreeItemId newId = event.GetItem();

	const NodePtr& oldNode = getNodeFromTreeId( oldId );
	const NodePtr& newNode = getNodeFromTreeId( newId );
	
	if( newNode )
		buttonNodeDelete->Enable();
	else
		buttonNodeDelete->Disable();

	sentLastSelectionEvent = true;

	// Send events to plugins.
	Events* events = editor->getEventManager();

	if( oldNode )
		events->onNodeUnselect(oldNode);

	if( newNode )
		events->onNodeSelect(newNode);

	sentLastSelectionEvent = false;
}

//-----------------------------------//

void ScenePage::onButtonNodeAdd(wxCommandEvent&)
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

	treeCtrl->Expand( id );
	treeCtrl->SelectItem( id );
	treeCtrl->EditLabel( id );
}

//-----------------------------------//

void ScenePage::onButtonNodeDelete(wxCommandEvent&)
{	
	wxTreeItemId id = treeCtrl->GetSelection();
	
	NodePtr node = getNodeFromTreeId(id);
	assert( node != nullptr );
	assert( nodeIds[node] == id );

	nodeIds.erase(node);
	treeCtrl->Delete(id);
	weakScene.lock()->remove( node );

	editor->RefreshViewport();
}

//-----------------------------------//

void ScenePage::onButtonNodeDeleteUpdate(wxUpdateUIEvent& event)
{
	ScenePtr scene( weakScene );
	bool empty = scene->getChildren().empty();
	
	event.Enable( !empty );
}

//-----------------------------------//

void ScenePage::onItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	const NodePtr& node = getNodeFromTreeId( menuItemId );

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

void ScenePage::onNodeMenu( wxCommandEvent& event )
{
	int id = event.GetId();

	if( id == ID_MenuSceneNodeVisible )
	{
		const NodePtr& node = getNodeFromTreeId( menuItemId );
		
		if( !node )
			return;
		
		node->setVisible( !node->isVisible() );
		editor->RefreshViewport();
	}

	if( id == ID_MenuSceneNodeWireframe )
	{
		const NodePtr& node = getNodeFromTreeId( menuItemId );
		
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

		editor->RefreshViewport();
	}

	if( id > ID_MenuSceneNodeComponentRangeStart
		&& id < ID_MenuSceneNodeComponentRangeEnd )
	{
		onComponentAdd( event );
	}
}

//-----------------------------------//

void ScenePage::onComponentAdd(wxCommandEvent& event )
{
	int id = event.GetId();
	const NodePtr& node = getNodeFromTreeId( menuItemId );

	if( id == ID_MenuSceneNodeMesh )
	{
		wxFileDialog fd( this, wxFileSelectorPromptStr,
			wxEmptyString, wxEmptyString, "Mesh files (*.ms3d)|*.ms3d",
			wxFD_DEFAULT_STYLE|wxFD_FILE_MUST_EXIST );

		if( fd.ShowModal() == wxID_OK )
		{
			ResourceManager* rm = engine->getResourceManager();
			
			//FileSystem* fs = engine->getFileSystem();
			//fs->

			std::string filename( fd.GetPath() );
			std::vector<std::string> elems = String::split( filename, '\\' );

			MeshPtr mesh = rm->loadResource<Mesh>( elems.back() );

			if( !mesh )
				return;

			ModelPtr model( new Model(mesh) );

			node->addComponent( model );
			addComponent( menuItemId, model );
		}
	}

	else if( id == ID_MenuSceneNodeTransform )
	{
		if( node->getTransform() )
			return;

		node->addTransform();
		addComponent( menuItemId, node->getTransform() );
	}

	else if( id == ID_MenuSceneNodeFirstPersonCamera )
	{
		RenderDevice* renderDevice = engine->getRenderDevice();
		CameraPtr camera( new FirstPersonCamera(renderDevice) );
		node->addComponent( camera );
		addComponent( menuItemId, camera );
	}
}

//-----------------------------------//

void ScenePage::onNodeAdded( const GroupEvent& event )
{
	if( event.node->getTag(EditorTags::EditorOnly) )
		return;
	
	addNode(event.node);
}

//-----------------------------------//

void ScenePage::onNodeRemoved( const GroupEvent& event )
{
	//const std::string& type = component.second->getType();
	//AppendItem( id, type, icons[type] );
	Events* events = editor->getEventManager();
	events->onNodeUnselect( event.node );
}

//-----------------------------------//

void ScenePage::onLabelEditBegin( wxTreeEvent& event )
{
	wxTreeItemId item = event.GetItem();
	NodePtr node = getNodeFromTreeId( item );
	
	if( !node )
	{
		event.Veto();
	}
}

//-----------------------------------//

void ScenePage::onLabelEditEnd( wxTreeEvent& event )
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

	NodePtr node = getNodeFromTreeId( item );
	assert( node != nullptr );

	node->setName( std::string( label.c_str() ) );
}

//-----------------------------------//

void ScenePage::onDragBegin( wxTreeEvent& event )
{
	//event.Allow();

	// Save the dragged original item, for later processing...
	dragItemId = event.GetItem();
}

//-----------------------------------//

void ScenePage::onDragEnd( wxTreeEvent& event )
{
	// Get the drop point location and do an hit test to check if
	// it was was done in a valid tree item id location.
	
	wxPoint drag_point = event.GetPoint();
	wxTreeItemId drag_id = treeCtrl->HitTest( drag_point );

	// If the drop was not done in a valid tree location, 
	// then we've got nothing to do here, move along...
	if( !drag_id.IsOk() )
		return;

	const NodePtr& src = getNodeFromTreeId( dragItemId );
	const NodePtr& dest = getNodeFromTreeId( drag_id );

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

void ScenePage::onMouseRightUp( wxContextMenuEvent& event )
{
	wxPoint clientpt = event.GetPosition();

	wxMenu menu("Scene");
	//menu.Append(ID_MenuSceneNodeDelete, "&Delete...");

	PopupMenu(&menu, clientpt);
}

//-----------------------------------//

} } // end namespaces