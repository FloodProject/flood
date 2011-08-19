/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ScenePane.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"
#include "Core/Utilities.h"
#include "EditorTags.h"
#include "UndoManager.h"
#include "Document.h"

#include "Scene/Tags.h"
#include "Scene/Scene.h"
#include "Scene/Component.h"
#include "Scene/Model.h"
#include "Scene/Skydome.h"
#include "Resources/Mesh.h"
#include "Terrain/Terrain.h"

#include "Plugins/Networking/ServerPlugin.h"
#include "Protocol/ReplicaMessages.h"
#include "Protocol/ReplicaContext.h"
#include "Network/Host.h"

#include "SceneDocument.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

ScenePage::ScenePage( wxWindow* parent, wxWindowID id )
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
	, treeCtrl(nullptr)
	, imageList(nullptr)
	, buttonEntityAdd(nullptr)
	, buttonEntityDelete(nullptr)
	, currentMenu(nullptr)
	, sentLastSelectionEvent(false)
	, entityCounter(0)
{
	initTree();
	initButtons();
	initIcons();
}

//-----------------------------------//

ScenePage::~ScenePage()
{
	treeCtrl->Destroy();
	cleanScene();
}

//-----------------------------------//

void ScenePage::cleanScene()
{
	ScenePtr scene = weakScene;

	if( !scene )
		return;
	
	scene->onEntityAdded.Disconnect( this, &ScenePage::onEntityAdded );
	scene->onEntityRemoved.Disconnect( this, &ScenePage::onEntityRemoved );

	weakScene = nullptr;
}

//-----------------------------------//

void ScenePage::initTree()
{
	sizer = new wxBoxSizer( wxVERTICAL );

	treeCtrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS |
		wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT /*| wxBORDER_NONE*/ );

	sizer->Add( treeCtrl, wxSizerFlags().Expand().Proportion(1) );

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

void ScenePage::addGroup( wxTreeItemId id, const EntityPtr& entity, bool createGroup )
{
	if( !entity ) return;

	if( !ClassInherits(entity->getType(), ReflectionGetType(Group)) )
	{
		addEntity(id, entity);
		return;
	}
	
	GroupPtr group = RefCast<Group>(entity);

	group->onEntityAdded.Connect( this, &ScenePage::onEntityAdded );
	group->onEntityRemoved.Connect( this, &ScenePage::onEntityRemoved );

	wxTreeItemId groupId = id;
		
	if( createGroup )
		groupId = treeCtrl->AppendItem( id, group->getName(), 0 );

	EntityItemData* data = new EntityItemData();
	data->entity = entity.get();

	entityIds[entity.get()] = groupId;
	treeCtrl->SetItemData( groupId, data );

	const std::vector<EntityPtr>& entities = group->getEntities();

	for( size_t i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& child = entities[i];
		addGroup(groupId, child);
	}
}

//-----------------------------------//

wxTreeItemId ScenePage::addEntity( wxTreeItemId id, const EntityPtr& entity )
{
	wxTreeItemId nodeId = treeCtrl->AppendItem( id, entity->getName(), 0 );

	const ComponentMap& components = entity->getComponents();

	ComponentMap::const_iterator it;
	
	for( it = components.begin(); it != components.end(); it++ )
		addComponent(nodeId, it->second);

	EntityItemData* data = new EntityItemData();
	data->entity = entity.get();

	treeCtrl->SetItemData( nodeId, data );
	entityIds[entity.get()] = nodeId;

	return nodeId;
}

//-----------------------------------//

void ScenePage::addComponent( wxTreeItemId id, ComponentPtr component )
{
	Class* klass = component->getType();
	wxTreeItemId compId = treeCtrl->AppendItem( id, klass->name, icons[klass] );

	EntityItemData* data = new EntityItemData();
	data->component = component;

	treeCtrl->SetItemData( compId, data );
}

//-----------------------------------//

ComponentPtr ScenePage::getComponentFromTreeId( wxTreeItemId id )
{
	if( !id ) return nullptr;
	EntityItemData* data = (EntityItemData*) treeCtrl->GetItemData(id);
	return data->component;
}

//-----------------------------------//

EntityPtr ScenePage::getEntityFromTreeId( wxTreeItemId id )
{
	if( !id ) return nullptr;

	EntityItemData* data = (EntityItemData*) treeCtrl->GetItemData(id);
	Entity* entity = data->entity;
	
	if( entity )
		return entity;
	else
		return nullptr;
}

//-----------------------------------//

wxTreeItemId ScenePage::getTreeIdFromEntity(const EntityPtr& entity)  
{
	Entity* raw = entity.get();

	EntityIdsMap::iterator it = entityIds.find(raw);
	if( it == entityIds.end() ) return nullptr;

	return entityIds[raw];
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
	Events* events = GetEditor().getEventManager();

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

void ScenePage::addEntity(const EntityPtr& entity)
{
	Document* document = GetEditor().getDocument();
	if( !document ) return;

	EntityOperation* entityOperation;
	entityOperation = createEntityOperation(entity, "Entity added");
	entityOperation->added = true;

	if( !entityOperation ) return;

	UndoManager* undoManager = document->getUndoManager();
	undoManager->registerOperation(entityOperation);

	entityOperation->redo();

	wxTreeItemId id = getTreeIdFromEntity(entity);
	treeCtrl->Expand(id);
	treeCtrl->SelectItem(id);
	treeCtrl->EditLabel(id);
}

//-----------------------------------//

void ScenePage::onButtonEntityAdd(wxCommandEvent&)
{
	String name("Entity"+StringFromNumber(entityCounter++));

	EntityPtr entity = EntityCreate( AllocatorGetHeap() );
	entity->setName(name);
	entity->addTransform();

#ifndef NO_NETWORK
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();

	ReplicaObjectCreateMessage inst;
	inst.contextId = document->replicaContext->id;
	inst.localId = 23;
	inst.classId = entity->getType()->id;
	inst.instance = entity.get();

	MessagePtr msg = MessageCreate( ReplicaMessageIds::ReplicaObjectCreate );
	msg->write(&inst);

	ServerPlugin* sp = GetPlugin<ServerPlugin>();
	sp->host->getPeer()->queueMessage(msg, 0);
#else
	addEntity(entity);
#endif
}

//-----------------------------------//

void ScenePage::onButtonEntityDelete(wxCommandEvent&)
{	
	wxTreeItemId id = treeCtrl->GetSelection();
	const EntityPtr& entity = getEntityFromTreeId(id);

	if(!entity)
		return;

	EntityOperation* entityOperation;
	entityOperation = createEntityOperation(entity, "Entity removed");
	entityOperation->added = false;

	if( !entityOperation )
		return;

	Events* events = GetEditor().getEventManager();
	events->onEntityUnselect(entity);

	entityOperation->redo();

#ifndef NO_NETWORK
	ReplicaObjectDeleteMessage del;
	del.instanceId = 0;

	MessagePtr msg = MessageCreate( ReplicaMessageIds::ReplicaObjectDelete );
	msg->write(&del);

	ServerPlugin* sp = GetPlugin<ServerPlugin>();
	sp->host->getPeer()->queueMessage(msg, 0);
#endif
}

//-----------------------------------//

void ScenePage::onButtonEntityDeleteUpdate(wxUpdateUIEvent& event)
{
	ScenePtr scene = weakScene;
	if( !scene ) return;

	bool empty = scene->getEntities().empty();
	event.Enable( !empty );
}

//-----------------------------------//

void ScenePage::onEntityAdded( const EntityPtr& entity )
{
	if( entity->getTag(EditorTags::EditorOnly) )
		return;
	
	wxTreeItemId id = getTreeIdFromEntity(entity->getParent());
	addGroup( id, entity );
}

//-----------------------------------//

void ScenePage::onEntityRemoved( const EntityPtr& entity )
{
	wxTreeItemId id = getTreeIdFromEntity(entity);
	treeCtrl->Delete(id);
	
	assert( entityIds[entity.get()] == id );
	entityIds.erase(entity.get());
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
	for( size_t i = 0; i < anims.size(); i++ )
	{
		const AnimationPtr& animation = anims[i];
		
		const String& name = animation->getName();
		if( name.empty() ) continue;

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
	for( size_t i = 0; i < bones.size(); i++ )
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

	Class* klass = component->getType();
	menu.SetTitle( klass->name );

	if(ReflectionIsEqual(klass, ReflectionGetType(Model)))
	{
		model = RefCast<Model>(component);
		meshHandle = model->getMesh();

		mesh = meshHandle.Resolve();

		if( !mesh ) return;

		if( !mesh->isAnimated() )
			return;

		wxMenu* menuAnimation = createMenuAnimation(mesh);
		menu.AppendSubMenu(menuAnimation, "Animation");

		wxMenu* menuAttachment = createMenuAttachment(mesh);
		menu.AppendSubMenu(menuAttachment, "Attachment");
	}
	else if(ReflectionIsEqual(klass, ReflectionGetType(Transform)))
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

	String name = "Attachment"+StringFromNumber(entityCounter++);
	
	EntityPtr entity( EntityCreate( AllocatorGetHeap() ) );
	entity->setName(name);
	entity->addTransform();

	ModelPtr model( Allocate(Model, AllocatorGetHeap(), meshHandle) );
	entity->addComponent(model);
	
	ScenePtr scene = weakScene;
	scene->add( entity );

	model->setAttachment( bone->name, entity );
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
	if( id == wxID_NONE ) return;

	ScenePtr scene = weakScene;
	const EntityPtr& node = getEntityFromTreeId( menuItemId );

	if( id == ID_MenuSceneEntityVisible )
	{
		if( !node ) return;
		node->setVisible( !node->isVisible() );
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityDuplicate )
	{
		if( !node ) return;

		#pragma TODO(Add object cloning)

#if 0
		Class& type = (Class&) node->getType();
		EntityPtr newEntity( type.clone<Entity>(node.get()) );

		const ComponentMap& components = node->getComponents();
		
		ComponentMap::const_iterator it;
		for( it = components.begin(); it != components.end(); it++ )
		{
			ComponentPtr component = it->second;
			Class& ctype = (Class&) component->getType();

			ComponentPtr newComponent( ctype.clone<Component>(component.get()) );
			newEntity->addComponent(newComponent);
		}
		
		scene->add(newEntity);
#endif
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityWireframe )
	{
		if( !node ) return;

		PolygonMode::Enum mode = event.IsChecked()
			? PolygonMode::Wireframe : PolygonMode::Solid;

		const std::vector<GeometryPtr>& geometries = node->getGeometry();

		for( size_t i = 0; i < geometries.size(); i++ )
		{
			const GeometryPtr& geo = geometries[i];

			const std::vector<RenderablePtr>& rends = geo->getRenderables();
			
			for( size_t j = 0; j < rends.size(); j++ )
			{
				const RenderablePtr& rend = rends[i];
				rend->setPolygonMode( mode );
			}
		}
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityDelete )
	{
		EntityOperation* entityOperation;
		entityOperation = createEntityOperation(node, "Entity Delete");
		entityOperation->undo();

#if 0
		String str = (String) treeCtrl->GetItemText(menuItemId);

		TypeRegistry& typeRegistry = TypeRegistry::getInstance();
		const Class* type = (Class*) typeRegistry.getType(str);
		assert( type != nullptr );

		if( !type->inherits<Component>() )
			return;

		const ComponentMap& components = node->getComponents();
		ComponentMap::const_iterator it = components.find(type);

		if( it == components.end() )
			return;

		ComponentPtr component = it->second;
		node->removeComponent(component);
#endif
	}
	//-----------------------------------//
	else if( id == ID_MenuSceneEntityTerrain )
	{
		String name("Terrain"+StringFromNumber(entityCounter++));
		
		TerrainPtr terrain( new Terrain(name) );
		scene->add( terrain );

		terrain->addCell(0, 0);
	}
	//-----------------------------------//
	else
	{
		onComponentAdd( event );
	}

	GetEditor().redrawView();
}

//-----------------------------------//

void ScenePage::onComponentAdd(wxCommandEvent& event )
{
	Entity* entity = getEntityFromTreeId( menuItemId ).get();
	
	int id = event.GetId();
	if( id == wxID_NONE ) return;

	const String name = currentMenu->GetLabelText(id); 
	Type* type = ReflectionFindType( name.c_str() );
	
	if( !type ) return;

	Class* klass = (Class*) type;
	Component* component = (Component*) ClassCreateInstance(klass, AllocatorGetHeap());
	
	if( !component )
	{
		LogAssert("Invalid component type");
		return;
	}

#ifndef NO_NETWORK
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();

	ReplicaInstanceId instanceId;
	if( !document->replicaContext->findInstance(entity, instanceId) )
	{
		LogDebug("Entity instance not found replica context");
		return;
	}

	ReplicaObjectCreateMessage create;
	create.contextId = document->replicaContext->id;
	create.parentId = instanceId;
	create.instance = component;

	MessagePtr m_create = MessageCreate( ReplicaMessageIds::ReplicaObjectCreate );
	m_create->write(&create);

	ServerPlugin* sp = GetPlugin<ServerPlugin>();
	sp->host->getPeer()->queueMessage(m_create, 0);
#else
	if( !entity->addComponent(component) )
	{
		LogDebug("Could not add component to entity");
		return;
	}
	
	addComponent(menuItemId, component);
#endif
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

	const EntityPtr& entity = getEntityFromTreeId( item );
	assert( entity != nullptr );

	entity->setName( std::string( label.c_str() ) );
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
		src->setParent( dest.get() );
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

NAMESPACE_EDITOR_END