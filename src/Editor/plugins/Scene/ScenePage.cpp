/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ScenePage.h"
#include "Utilities.h"
#include "Events.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Viewframe.h"
#include "UndoManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

void EntityOperation::redo()
{
	ScenePtr scene = weakScene.lock();
	if(scene) scene->add(node);
}

void EntityOperation::undo()
{
	ScenePtr scene = weakScene.lock();
	if(scene) scene->remove(node);
}

//-----------------------------------//

class EntityItemData : public wxTreeItemData
{
public:

	EntityWeakPtr node;
	ComponentWeakPtr component;
};

//-----------------------------------//

enum 
{
	ID_MenuSceneEntityDelete = wxID_HIGHEST+472,
	ID_MenuSceneEntityDuplicate,
	ID_MenuSceneEntityVisible,
	ID_MenuSceneEntityWireframe,
	ID_MenuSceneEntityTerrain,
};

//-----------------------------------//

struct ComponentEntry
{
	bool show;
	const Type& type;
	const unsigned char* icon;
	const int icon_length;
};

#define TYPE(t) t::getType()
#define BMP(s) s, sizeof(s)

static ComponentEntry components[] = {
	{ true, TYPE(Transform),			BMP(chart_line) },
	{ true, TYPE(Model),				BMP(shape_flip_horizontal) },
	{ true, TYPE(Light),				BMP(lightbulb_off) },
	{ true, TYPE(Skydome),				BMP(weather_clouds) },
	{ true, TYPE(Camera),				BMP(camera) },
	//{ true, TYPE(Sound),				BMP(sound) },
	//{ true, TYPE(Listener),			BMP(status_online) },
	{ true, TYPE(Grid),					BMP(grid_icon_white_bg) },
	{ true, TYPE(Geometry),				BMP(shape_flip_horizontal) },
	{ true, TYPE(FirstPersonController),BMP(camera) },
	{ true, TYPE(ThirdPersonController),BMP(camera) },

#ifdef VAPOR_SCRIPTING_LUA
	{ true, TYPE(ScriptController),		BMP(shape_flip_horizontal) },
#endif

#ifdef VAPOR_PHYSICS_BULLET
	{ true, TYPE(CharacterController),	BMP(link) },
	{ true, TYPE(BoxShape),				BMP(link) },
	{ true, TYPE(MeshShape),			BMP(link) },
	{ true, TYPE(CapsuleShape),			BMP(link) },
	{ true, TYPE(Body),					BMP(link) },
#endif

	{ false,TYPE(Scene),				BMP(sitemap_color) }
};

//-----------------------------------//	

template<typename T> T* cloneObject(T* object)
{
	const Class& type = object->getInstanceType();
	T* newObject = (T*) type.createInstance();

	const FieldsMap& fields = type.getFields();
	
	FieldsMap::const_iterator it;
	for( it = fields.cbegin(); it != fields.cend(); it++ )
	{
		Field* field = it->second;
		const Type& field_type = field->type;
		
		if( field->isPointer() )
		{
			field->set<ResourcePtr>(newObject, field->get<ResourcePtr>(object));
		}
		else if( field_type.isPrimitive() )
		{
			const Primitive& type = (Primitive&) field_type;

			if( type.isBool() )
			{
				field->set<bool>(newObject, field->get<bool>(object));
			}
			//-----------------------------------//
			else if( type.isInteger() )
			{
				field->set<int>(newObject, field->get<int>(object));
			}
			//-----------------------------------//
			else if( type.isFloat() )
			{
				field->set<float>(newObject, field->get<float>(object));
			}
			//-----------------------------------//
			else if( type.isString() )
			{
				field->set<std::string>(newObject, field->get<std::string>(object));
			}
			//-----------------------------------//
			else if( type.isColor() )
			{
				field->set<Color>(newObject, field->get<Color>(object));
			}
			//-----------------------------------//
			else if( type.isVector3() )
			{
				field->set<Vector3>(newObject, field->get<Vector3>(object));
			}
		}
	}

	return newObject;
}

//-----------------------------------//

ScenePage::ScenePage( EditorFrame* frame, wxWindow* parent, wxWindowID id )
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, -1) )
	, treeCtrl(nullptr)
	, imageList(nullptr)
	, buttonEntityAdd(nullptr)
	, buttonEntityDelete(nullptr)
	, currentMenu(nullptr)
	, sentLastSelectionEvent(false)
	, editor(frame)
	, engine(editor->getEngine())
	, nodeCounter(0)
{
	initTree();
	initButtons();
	initIcons();
}

//-----------------------------------//

ScenePage::~ScenePage()
{
	cleanScene();
}

//-----------------------------------//

void ScenePage::cleanScene()
{
	ScenePtr scene = weakScene.lock();

	if( !scene )
		return;
	
	scene->onEntityAdded.Disconnect( this, &ScenePage::onEntityAdded );
	scene->onEntityRemoved.Disconnect( this, &ScenePage::onEntityRemoved );

	weakScene.reset();
}

//-----------------------------------//

void ScenePage::initTree()
{
	sizer = new wxBoxSizer( wxVERTICAL );

	treeCtrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS 
		| wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT );

	sizer->Add( treeCtrl, 1, wxEXPAND, 0 );

	treeCtrl->Bind(wxEVT_COMMAND_TREE_ITEM_MENU, &ScenePage::onItemMenu, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_SEL_CHANGED, &ScenePage::onItemChanged, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, &ScenePage::onLabelEditBegin, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_END_LABEL_EDIT, &ScenePage::onLabelEditEnd, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_BEGIN_DRAG, &ScenePage::onDragBegin, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_END_DRAG, &ScenePage::onDragEnd, this);

	SetSizer( sizer ); 

	Bind(wxEVT_CONTEXT_MENU, &ScenePage::onContextMenu, this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &ScenePage::onMenuSelected, this);
}

//-----------------------------------//

void ScenePage::initButtons()
{
	wxBoxSizer* sizer2 = new wxBoxSizer( wxHORIZONTAL );

	// Search control.
	wxStaticBitmap* iconFind = new wxStaticBitmap( this, wxID_ANY, wxMEMORY_BITMAP(find) );
	sizer2->Add( iconFind, 0, wxALL|wxEXPAND, 5 );
	
	wxTextCtrl* textFind = new wxTextCtrl( this, wxID_ANY );
	sizer2->Add( textFind, 1, wxEXPAND|wxALL, 5 );
	
	// Entity delete button.
	buttonEntityDelete = new wxBitmapButton( this, wxID_ANY, wxMEMORY_BITMAP(package_delete) );
	buttonEntityDelete->SetBitmapDisabled( wxMEMORY_BITMAP(package_delete_disable) );
	sizer2->Add( buttonEntityDelete, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	buttonEntityDelete->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ScenePage::onButtonEntityDelete, this);
	buttonEntityDelete->Bind(wxEVT_UPDATE_UI, &ScenePage::onButtonEntityDeleteUpdate, this);

	// Entity add button.
	buttonEntityAdd = new wxBitmapButton( this, wxID_ANY, wxMEMORY_BITMAP(package_add) );
	sizer2->Add( buttonEntityAdd, 0, wxTOP|wxBOTTOM, 5 );

	buttonEntityAdd->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ScenePage::onButtonEntityAdd, this);
	sizer->Add( sizer2, 0, wxEXPAND, 0 );
}

//-----------------------------------//

void ScenePage::initIcons()
{
	// Create a new list with all the icons.
	imageList = new wxImageList(16, 16, false, 16);
	imageList->Add(wxMEMORY_BITMAP(package));

	for( uint i = 0; i < VAPOR_ARRAY_SIZE(components); i++ )
	{
		const ComponentEntry& c = components[i];
		const Type* type = &c.type;
		
		bitmaps[type] = _wxConvertMemoryToBitmap(c.icon, c.icon_length);
		icons[type] = imageList->Add(bitmaps[type]);
	}

	treeCtrl->AssignImageList(imageList);
}

//-----------------------------------//

void ScenePage::setScene(const ScenePtr& scene)
{
	cleanScene();

	weakScene = scene;

	// Add the root node.
	treeCtrl->DeleteAllItems();

	wxString sceneName( scene->getName() );
	rootId = treeCtrl->AddRoot(sceneName.Capitalize(), 1);

	addGroup( rootId, scene, false );
}

//-----------------------------------//

void ScenePage::addGroup( wxTreeItemId id, const EntityPtr& node, bool createGroup )
{
	if( !node->getInstanceType().inherits<Group>() )
	{
		addEntity(id, node);
		return;
	}
	
	GroupPtr group = std::static_pointer_cast<Group>(node);

	group->onEntityAdded.Connect( this, &ScenePage::onEntityAdded );
	group->onEntityRemoved.Connect( this, &ScenePage::onEntityRemoved );

	wxTreeItemId groupId = id;
		
	if( createGroup )
		groupId = treeCtrl->AppendItem( id, group->getName(), 0 );

	EntityItemData* data = new EntityItemData();
	data->node = node;

	nodeIds[node] = groupId;
	treeCtrl->SetItemData( groupId, data );

	for( uint i = 0; i < group->getEntities().size(); i++ )
	{
		const EntityPtr& child = group->getEntities()[i];
		addGroup(groupId, child);
	}
}

//-----------------------------------//

wxTreeItemId ScenePage::addEntity( wxTreeItemId id, const EntityPtr& node )
{
	wxTreeItemId nodeId = treeCtrl->AppendItem( id, node->getName(), 0 );

	const ComponentMap& components = node->getComponents();

	ComponentMap::const_iterator it;
	for( it = components.cbegin(); it != components.cend(); it++ )
		addComponent(nodeId, it->second);

	EntityItemData* data = new EntityItemData();
	data->node = node;

	treeCtrl->SetItemData( nodeId, data );
	nodeIds[node] = nodeId;

	return nodeId;
}

//-----------------------------------//

void ScenePage::addComponent( wxTreeItemId id, ComponentPtr component )
{
	assert( component != nullptr );
	const Type& type = component->getInstanceType();

	wxTreeItemId compId = treeCtrl->AppendItem( id, type.getName(), icons[&type] );

	EntityItemData* data = new EntityItemData();
	data->component = component;

	treeCtrl->SetItemData( compId, data );
}

//-----------------------------------//

ComponentPtr ScenePage::getComponentFromTreeId( wxTreeItemId id )
{
	if( !id )
		return ComponentPtr();

	EntityItemData* data = (EntityItemData*) treeCtrl->GetItemData(id);
	assert( data != nullptr );

	return data->component.lock();
}

//-----------------------------------//

EntityPtr ScenePage::getEntityFromTreeId( wxTreeItemId id )
{
	if( !id )
		return EntityPtr();

	EntityItemData* data = (EntityItemData*) treeCtrl->GetItemData(id);
	assert( data != nullptr );

	return data->node.lock();
}

//-----------------------------------//

wxTreeItemId ScenePage::getTreeIdFromEntity(const EntityPtr& node)  
{
	EntityIdsMap::iterator it = nodeIds.find(node);

	if( it == nodeIds.end() )
		return wxTreeItemId();

	return nodeIds[node];
}

//-----------------------------------//

void ScenePage::onItemChanged(wxTreeEvent& event)
{
	wxTreeItemId oldId = event.GetOldItem();
	wxTreeItemId newId = event.GetItem();

	const EntityPtr& oldEntity = getEntityFromTreeId( oldId );
	const EntityPtr& newEntity = getEntityFromTreeId( newId );
	
	const ComponentPtr& oldComponent = getComponentFromTreeId( oldId );
	const ComponentPtr& newComponent = getComponentFromTreeId( newId );

	sentLastSelectionEvent = true;

	// Send events to plugins.
	Events* events = editor->getEventManager();

	if( oldEntity )
		events->onEntityUnselect(oldEntity);

	if( oldComponent )
		events->onComponentUnselect(oldComponent);

	if( newEntity )
		events->onEntitySelect(newEntity);

	if( newComponent )
		events->onComponentSelect(newComponent);

	sentLastSelectionEvent = false;
}

//-----------------------------------//

void ScenePage::onButtonEntityAdd(wxCommandEvent&)
{
	std::string name("Entity"+String::fromNumber(nodeCounter++));
	EntityPtr node( new Entity(name) );
	node->addTransform();
	
	EntityOperation* nodeOperation = new EntityOperation();
	nodeOperation->node = node;
	nodeOperation->weakScene = weakScene;

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation(nodeOperation);

	nodeOperation->redo();
	
	wxTreeItemId id = getTreeIdFromEntity(node);
	treeCtrl->Expand(id);
	treeCtrl->SelectItem(id);
	treeCtrl->EditLabel(id);

	editor->redrawView();
}

//-----------------------------------//

void ScenePage::onButtonEntityDelete(wxCommandEvent&)
{	
	wxTreeItemId id = treeCtrl->GetSelection();
	const EntityPtr& node = getEntityFromTreeId(id);

	EntityOperation* nodeOperation = createEntityOperation(node);

	if( !nodeOperation )
		return;

	nodeOperation->undo();

	editor->redrawView();
}

//-----------------------------------//

void ScenePage::onButtonEntityDeleteUpdate(wxUpdateUIEvent& event)
{
	ScenePtr scene = weakScene.lock();

	if( !scene )
		return;

	bool empty = scene->getEntities().empty();
	event.Enable( !empty );
}

//-----------------------------------//

EntityOperation* ScenePage::createEntityOperation(const EntityPtr& node)
{
	if( !node )
		return nullptr;

	EntityOperation* nodeOperation = new EntityOperation();
	nodeOperation->node = node;
	nodeOperation->weakScene = weakScene;

	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation(nodeOperation);

	return nodeOperation;
}

//-----------------------------------//

void ScenePage::onEntityAdded( const EntityPtr& node )
{
	if( node->getTag(EditorTags::EditorOnly) )
		return;
	
	wxTreeItemId id = getTreeIdFromEntity(node->getParent());
	addGroup( id, node );
}

//-----------------------------------//

void ScenePage::onEntityRemoved( const EntityPtr& node )
{
	wxTreeItemId id = getTreeIdFromEntity(node);
	treeCtrl->Delete(id);
	
	assert( nodeIds[node] == id );
	nodeIds.erase(node);

	//Events* events = editor->getEventManager();
	//events->onEntityUnselect( event.node );
}

//-----------------------------------//

void ScenePage::populateEntityItemMenu(wxMenu& menu, const EntityPtr& node)
{
	menu.AppendCheckItem(ID_MenuSceneEntityVisible, "&Visible");
	menu.Check(ID_MenuSceneEntityVisible, node->isVisible() );

	const std::vector<GeometryPtr>& geo = node->getGeometry();
	
	if( !geo.empty() )
	{
		const std::vector<RenderablePtr>& rend = geo.front()->getRenderables();
		
		if( !rend.empty() )
		{
			bool state = (rend.front()->getPolygonMode() != PolygonMode::Solid);
			menu.AppendCheckItem(ID_MenuSceneEntityWireframe, "&Wireframe");
			menu.Check(ID_MenuSceneEntityWireframe, state );
		}
	}

	menu.Append(ID_MenuSceneEntityDelete, "&Delete");
	menu.Append(ID_MenuSceneEntityDuplicate, "Duplicate");

	menu.AppendSeparator();

	for( uint i = 0; i < VAPOR_ARRAY_SIZE(components); i++ )
	{
		ComponentEntry& c = components[i];
		const Type& type = c.type;

		wxMenuItem* item = menu.Append(wxID_ANY, type.getName());
		item->SetBitmap( bitmaps[&type], false );
	}
}

//-----------------------------------//

wxMenu* ScenePage::createMenuAnimation(const MeshPtr& mesh)
{
	wxMenu* menuAnimation = new wxMenu();

	if( mesh->getAnimations().empty() )
		return menuAnimation;

	wxMenuItem* itemFirst = nullptr;
	wxMenuItem* item = nullptr;

	const std::vector<AnimationPtr>& anims = mesh->getAnimations();
	for( uint i = 0; i < anims.size(); i++ )
	{
		const AnimationPtr& animation = anims[i];
		const std::string& name = animation->getName();

		if( name.empty() )
			continue;

		int id = wxNewId();
		item = menuAnimation->Append(id, name);

		if( !itemFirst )
			itemFirst = item;
	}

	if( !itemFirst )
		return menuAnimation;
	
	firstAnimationId = itemFirst->GetId();

	Bind( wxEVT_COMMAND_MENU_SELECTED,
		&ScenePage::onAnimationMenuSelected, this,
		itemFirst->GetId(), item->GetId() );

	return menuAnimation;
}

//-----------------------------------//

wxMenu* ScenePage::createMenuAttachment(const MeshPtr& mesh)
{
	wxMenu* menuAttachment = new wxMenu();

	wxMenuItem* itemFirst = nullptr;
	wxMenuItem* item = nullptr;

	const std::vector<BonePtr>& bones = mesh->getSkeleton()->getBones();
	for( uint i = 0; i < bones.size(); i++ )
	{
		const BonePtr& bone = bones[i];
		item = menuAttachment->Append( wxNewId(), bone->name );

		if( !itemFirst )
			itemFirst = item;
	}

	firstAttachmentId = itemFirst->GetId();

	Bind( wxEVT_COMMAND_MENU_SELECTED,
		&ScenePage::onAttachmentMenuSelected, this,
		itemFirst->GetId(), item->GetId() );

	return menuAttachment;
}

//-----------------------------------//

void ScenePage::populateComponentItemMenu(wxMenu& menu, const ComponentPtr& component)
{
	if( !component )
		return;

	const Type& type = component->getInstanceType();
	menu.SetTitle( type.getName() );

	if(type.is<Model>())
	{
		model = std::static_pointer_cast<Model>(component);
		mesh = model->getMesh();

		if( !mesh->isAnimated() )
			return;

		wxMenu* menuAnimation = createMenuAnimation(mesh);
		menu.AppendSubMenu(menuAnimation, "Animation");

		wxMenu* menuAttachment = createMenuAttachment(mesh);
		menu.AppendSubMenu(menuAttachment, "Attachment");
	}
	else if(type.is<Transform>())
	{
		//wxMenuItem* item = new wxMenuItem("");
	}
}

//-----------------------------------//

void ScenePage::onAnimationMenuSelected(wxCommandEvent& event)
{
	int id = event.GetId();
	int ind = id - firstAnimationId;
	
	AnimationPtr animation = mesh->getAnimations()[ind];
	model->setAnimation( animation );
}

//-----------------------------------//

void ScenePage::onAttachmentMenuSelected(wxCommandEvent& event)
{
	int id = event.GetId();
	int ind = id - firstAttachmentId;
	
	const SkeletonPtr& skeleton = mesh->getSkeleton();
	BonePtr bone = skeleton->getBones()[ind];

	MeshPtr mesh = askMeshResource();

	if( !mesh )
		return;

	std::string name = "Attachment"+String::fromNumber(nodeCounter++);
	
	EntityPtr node( new Entity(name) );
	node->addTransform();
	node->addComponent( ModelPtr( new Model(mesh) ) );
	
	ScenePtr scene = weakScene.lock();
	scene->add( node );

	model->setAttachment( bone->name, node );
}

//-----------------------------------//

void ScenePage::onItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();

	const EntityPtr& node = getEntityFromTreeId( menuItemId );
	const ComponentPtr& component = getComponentFromTreeId( menuItemId );

	wxMenu menu;
	currentMenu = &menu;

	if( node )
	{
		menu.SetTitle("Entity");
		populateEntityItemMenu(menu, node);
	}
	else
	{
		menu.SetTitle("Component");
		populateComponentItemMenu(menu, component);
	}

	wxPoint clientpt = event.GetPoint();
	PopupMenu(&menu, clientpt);
}

//-----------------------------------//

void ScenePage::onMenuSelected( wxCommandEvent& event )
{
	int id = event.GetId();

	if( id == wxID_NONE )
		return;

	ScenePtr scene = weakScene.lock();
	const EntityPtr& node = getEntityFromTreeId( menuItemId );

	if( id == ID_MenuSceneEntityVisible )
	{
		if( !node )
			return;

		node->setVisible( !node->isVisible() );
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityDuplicate )
	{
		if( !node )
			return;

		EntityPtr newEntity( cloneObject<Entity>(node.get()) );

		const ComponentMap& components = node->getComponents();
		
		ComponentMap::const_iterator it;
		for( it = components.cbegin(); it != components.cend(); it++ )
		{
			ComponentPtr component = it->second;
			ComponentPtr newComponent( cloneObject<Component>(component.get()) );
			newEntity->addComponent(newComponent);
		}
		
		scene->add(newEntity);
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityWireframe )
	{
		if( !node )
			return;

		PolygonMode::Enum mode = event.IsChecked() ? PolygonMode::Wireframe : PolygonMode::Solid;

		const std::vector<GeometryPtr>& geometries = node->getGeometry();

		for( uint i = 0; i < geometries.size(); i++ )
		{
			const GeometryPtr& geo = geometries[i];

			const std::vector<RenderablePtr>& rends = geo->getRenderables();
			
			for( uint j = 0; j < rends.size(); j++ )
			{
				const RenderablePtr& rend = rends[i];
				rend->setPolygonMode( mode );
			}
		}
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityDelete )
	{
		EntityOperation* op = createEntityOperation(node);
		op->undo();
		//std::string str = (std::string) treeCtrl->GetItemText(menuItemId);

		//TypeRegistry& typeRegistry = TypeRegistry::getInstance();
		//const Class* type = (Class*) typeRegistry.getType(str);
		//assert( type != nullptr );

		//if( !type->inherits<Component>() )
		//	return;

		//const ComponentMap& components = node->getComponents();
		//ComponentMap::const_iterator it = components.find(type);

		//if( it == components.end() )
		//	return;

		//ComponentPtr component = it->second;
		//node->removeComponent(component);
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityTerrain )
	{
		std::string name("Terrain"+String::fromNumber(nodeCounter++));
		
		TerrainPtr terrain( new Terrain(name) );
		scene->add( terrain );

		terrain->addCell(0, 0);
	}
	//-----------------------------------//
	else
	{
		onComponentAdd( event );
	}

	editor->redrawView();
}

//-----------------------------------//

MeshPtr ScenePage::askMeshResource()
{
	wxFileDialog fd( this, wxFileSelectorPromptStr,
			wxEmptyString, wxEmptyString, "Mesh files (*.ms3d)|*.ms3d",
			wxFD_DEFAULT_STYLE|wxFD_FILE_MUST_EXIST );

	if( fd.ShowModal() != wxID_OK )
		return nullptr;

	std::string file( fd.GetPath() );
	std::vector<std::string> elems = String::split(file, '\\');

	ResourceManager* rm = engine->getResourceManager();
	MeshPtr mesh = rm->loadResource<Mesh>( elems.back() );

	return mesh;
}

//-----------------------------------//

void ScenePage::onComponentAdd(wxCommandEvent& event )
{
	const EntityPtr& node = getEntityFromTreeId( menuItemId );
	
	int id = event.GetId();
	
	if( id == wxID_NONE )
		return;

	const wxString& name =  currentMenu->GetLabelText(id); 
	
	TypeRegistry& registry = TypeRegistry::getInstance();
	const Type* type = registry.getType( (std::string) name );
	
	if( !type )
		return;

	ComponentPtr component;

	if( type->is<Model>() )
	{
		MeshPtr mesh = askMeshResource();

		if( mesh )
			component.reset( new Model(mesh) );
	}
	else
	{
		Class& classType = (Class&) *type;
		component.reset( (Component*) classType.createInstance() );
	}

	if( component && node->addComponent(component) )
		addComponent(menuItemId, component);
}

//-----------------------------------//

void ScenePage::onLabelEditBegin( wxTreeEvent& event )
{
	wxTreeItemId item = event.GetItem();
	EntityPtr node = getEntityFromTreeId( item );
	
	if( !node )
		event.Veto();
}

//-----------------------------------//

void ScenePage::onLabelEditEnd( wxTreeEvent& event )
{
	wxTreeItemId item = event.GetItem();
	const wxString& label = event.GetLabel();

	if( event.IsEditCancelled() )
	{
		event.Veto();
		return;
	}

	#pragma TODO("Test for duplicate scene names")

	if( label.empty() )
	{
		event.Veto();
		return;
	}

	const EntityPtr& node = getEntityFromTreeId( item );
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
	
	wxPoint dragPoint = event.GetPoint();
	wxTreeItemId dragId = treeCtrl->HitTest( dragPoint );

	// If the drop was not done in a valid tree location, 
	// then we've got nothing to do here, move along...
	if( !dragId.IsOk() )
		return;

	const EntityPtr& src = getEntityFromTreeId( dragItemId );
	const EntityPtr& dest = getEntityFromTreeId( dragId );

	if( src && dest ) // It is a node, just re-parent it.
	{
		src->setParent( dest );
	}
	else // It was probably a component, so handle it.
	{
		#pragma TODO("Move component from one node to another")
	}

	//DeleteChildren( root );
	//updateScene( root, scene );
}

//-----------------------------------//

void ScenePage::onContextMenu( wxContextMenuEvent& event )
{
	menuItemId = wxTreeItemId();

	wxMenu menu("Scene");
	menu.Append(ID_MenuSceneEntityTerrain, "Create &Terrain");

	PopupMenu(&menu);
}

//-----------------------------------//

} } // end namespaces