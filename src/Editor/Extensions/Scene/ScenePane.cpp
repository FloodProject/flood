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
#include "EventManager.h"
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

#include "Plugins/Selection/SelectionPlugin.h"
#include "Plugins/Selection/SelectionManager.h"
#include "Plugins/Selection/SelectionOperation.h"

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
	, sentTreeChangingEvent(false)
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
}

//-----------------------------------//

void ScenePage::cleanScene()
{
	LogDebug("Cleaning scene in ScenePane");

	treeCtrl->DeleteAllItems();
	objectIds.clear();

	model.reset();
	meshHandle.reset();

	ScenePtr scene = weakScene;
	if( !scene ) return;
	
	scene->entities.onEntityAdded.Disconnect( this, &ScenePage::onEntityAdded );
	scene->entities.onEntityRemoved.Disconnect( this, &ScenePage::onEntityRemoved );

	scene->entities.onEntityComponentAdded.Disconnect( this, &ScenePage::onComponentAdded );
	scene->entities.onEntityComponentRemoved.Disconnect( this, &ScenePage::onComponentRemoved );

	weakScene.reset();
	weakEntity.reset();
	weakComponent.reset();
}

//-----------------------------------//

void ScenePage::initTree()
{
	sizer = new wxBoxSizer( wxVERTICAL );

	treeCtrl = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS |
		wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT | wxBORDER_NONE );

	sizer->Add( treeCtrl, wxSizerFlags().Expand().Proportion(1) );

	treeCtrl->Bind(wxEVT_COMMAND_TREE_ITEM_MENU, &ScenePage::onItemMenu, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_SEL_CHANGED, &ScenePage::onItemChanged, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, &ScenePage::onLabelEditBegin, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_END_LABEL_EDIT, &ScenePage::onLabelEditEnd, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_BEGIN_DRAG, &ScenePage::onDragBegin, this);
	treeCtrl->Bind(wxEVT_COMMAND_TREE_END_DRAG, &ScenePage::onDragEnd, this);
	treeCtrl->Bind(wxEVT_SET_FOCUS , &ScenePage::onFocus, this);

	SetSizer( sizer ); 

	Bind(wxEVT_CONTEXT_MENU, &ScenePage::onContextMenu, this);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &ScenePage::onMenuSelected, this);

	wxStaticLine* line = new wxStaticLine(this);
	sizer->Add( line, wxSizerFlags().Expand());
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
	//assert( weakScene.get() == 0 );
	weakScene = scene;

	wxString sceneName = PathGetFileBase( scene->getPath() );
	rootId = treeCtrl->AddRoot(sceneName.Capitalize(), 1);

	addGroup( rootId, &scene->entities, false );
}

//-----------------------------------//

void ScenePage::addGroup( wxTreeItemId id, Entity* entity, bool createGroup )
{
	if( !entity || !id.IsOk() ) return;

	if( !ClassInherits(entity->getType(), ReflectionGetType(Group)) )
	{
		addEntity(id, entity);
		return;
	}
	
	Group* group = (Group*) entity;

	group->onEntityAdded.Connect( this, &ScenePage::onEntityAdded );
	group->onEntityRemoved.Connect( this, &ScenePage::onEntityRemoved );

	group->onEntityComponentAdded.Connect( this, &ScenePage::onComponentAdded );
	group->onEntityComponentRemoved.Connect( this, &ScenePage::onComponentRemoved );

	wxTreeItemId groupId = id;
		
	if( createGroup )
		groupId = treeCtrl->AppendItem( id, group->getName(), 0 );

	EntityItemData* data = new EntityItemData();
	data->entity = entity;

	objectIds[entity] = groupId;
	treeCtrl->SetItemData( groupId, data );

	const std::vector<EntityPtr>& entities = group->getEntities();

	for( size_t i = 0; i < entities.size(); i++ )
	{
		Entity* child = entities[i].get();
		addGroup(groupId, child);
	}
}

//-----------------------------------//

wxTreeItemId ScenePage::addEntity( wxTreeItemId id, Entity* entity )
{
	wxTreeItemId entityId = treeCtrl->AppendItem( id, entity->getName(), 0 );

	const ComponentMap& components = entity->getComponents();

	ComponentMap::const_iterator it;
	
	for( it = components.begin(); it != components.end(); it++ )
		addComponentToTree(entityId, it->second);

	EntityItemData* data = new EntityItemData();
	data->entity = entity;

	treeCtrl->SetItemData( entityId, data );
	objectIds[entity] = entityId;

	return entityId;
}

//-----------------------------------//

void ScenePage::addComponentToTree( wxTreeItemId id, ComponentPtr component )
{
	Class* klass = component->getType();
	wxTreeItemId compId = treeCtrl->AppendItem( id, klass->name, icons[klass] );

	EntityItemData* data = new EntityItemData();
	data->component = component;

	treeCtrl->SetItemData( compId, data );
	objectIds[ component.get() ] = compId;
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
	if( !data ) return nullptr;
	
	Entity* entity = data->entity;
	return entity;
}

//-----------------------------------//

wxTreeItemId ScenePage::getTreeIdFromObject(Object* object)
{
	ObjectIdsMap::iterator it = objectIds.find(object);
	if( it == objectIds.end() ) return nullptr;

	return objectIds[object];
}

//-----------------------------------//

void ScenePage::onFocus(wxFocusEvent& event)
{
	// Workaround for when there is already an unfocused selection.
	// If the user clicks on it then the event is not generated.

	wxPoint mouseRelPos = wxGetMousePosition() - GetScreenPosition();

	wxTreeItemId selItem = treeCtrl->GetSelection();
	wxTreeItemId hitItem = treeCtrl->HitTest(mouseRelPos);

	if(selItem == hitItem)
	{
		//treeCtrl->SelectItem(selItem);

		wxTreeEvent event;
		event.SetItem(selItem);

		onItemChanged(event);
	}

	event.Skip();
}

//-----------------------------------//

void ScenePage::onItemChanged(wxTreeEvent& event)
{
	LogDebug("Item changed in ScenePage");

	if( sentTreeChangingEvent )
	{
		sentTreeChangingEvent = false;
		return;
	}

	wxTreeItemId oldId = event.GetOldItem();
	wxTreeItemId newId = event.GetItem();

	const EntityPtr& oldEntity = getEntityFromTreeId( oldId );
	const EntityPtr& newEntity = getEntityFromTreeId( newId );
	
	const ComponentPtr& oldComponent = getComponentFromTreeId( oldId );
	const ComponentPtr& newComponent = getComponentFromTreeId( newId );

	// Send events to plugins.
	EventManager* events = GetEditor().getEventManager();

	if( oldComponent )
		events->onComponentUnselect(oldComponent);
	
	if( newComponent )
		events->onComponentSelect(newComponent);

	// Send an entity unselection event if the new entity being
	// selected is different than the old one being unselected.

	if( oldComponent && newComponent )
	{
		Entity* oldEntity = oldComponent->getEntity();
		Entity* newEntity = newComponent->getEntity();

		if( oldEntity != newEntity )
			events->onEntityUnselect(oldEntity);
	}

	SelectionManager* selections = GetPlugin<SelectionPlugin>()->getSelectionManager();
	if( !selections ) return;

	//const SelectionCollection& selected = selections->getSelections();
	//bool isSameSelection = selected && selected->isSelection(newEntity);
	
	if( newEntity /*&& !isSameSelection*/ )
	{
		SelectionOperation* selection = CreateEntitySelectionOperation(newEntity);
		selection->redo();

		UndoManager* undoManager = GetEditor().getDocument()->getUndoManager();
		undoManager->registerOperation(selection);
	}

	sentLastSelectionEvent = false;
}

//-----------------------------------//

EntityOperation* ScenePage::createEntityAddOperation(Entity* entity)
{
	Document* document = GetEditor().getDocument();
	if( !document ) return nullptr;

	EntityOperation* entityOperation;
	entityOperation = createEntityOperation("Entity added");
	entityOperation->entity = entity;
	entityOperation->type = EntityOperation::EntityAdded;
	entityOperation->redo();

	UndoManager* undoManager = document->getUndoManager();
	undoManager->registerOperation(entityOperation);

	return entityOperation;
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

	createEntityAddOperation(entity.get());

	// Simulate selection event but don't register it in the undo stack.
	SelectionOperation* selection = CreateEntitySelectionOperation(entity);
	if( !selection ) return;

	selection->redo();

	Deallocate(selection);

	// Don't use SelectItem() because it will generate an item selection event.
	wxTreeItemId id = getTreeIdFromObject(entity.get());
	
	if( id.IsOk() )
	{
		treeCtrl->Expand(id);

		sentTreeChangingEvent = true;
		treeCtrl->SelectItem(id);
	}
#endif
}

//-----------------------------------//

void ScenePage::onButtonEntityDelete(wxCommandEvent&)
{	
	wxTreeItemId id = treeCtrl->GetSelection();
	Document* document = GetEditor().getDocument();
	
	const EntityPtr& entity = getEntityFromTreeId(id);
	if(!entity) return;

	EntityOperation* entityOperation;
	entityOperation = createEntityOperation("Entity removed");
	entityOperation->entity = entity;
	entityOperation->type = EntityOperation::EntityRemoved;
	entityOperation->redo();
	
	UndoManager* undoManager = document->getUndoManager();
	undoManager->registerOperation(entityOperation);

	EventManager* events = GetEditor().getEventManager();
	events->onEntityUnselect(entity);

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

	bool empty = scene->entities.getEntities().empty();
	event.Enable( !empty );
}

//-----------------------------------//

void ScenePage::onEntityAdded( const EntityPtr& entity )
{
	if( entity->getTag(EditorTags::EditorOnly) )
		return;
	
	EntityPtr parent = entity->getParent();
	if( !parent ) return;

	wxTreeItemId id = getTreeIdFromObject(parent.get());
	addGroup( id, entity.get() );
}

//-----------------------------------//

void ScenePage::onEntityRemoved( const EntityPtr& entity )
{
	wxTreeItemId id = getTreeIdFromObject(entity.get());

	sentTreeChangingEvent = true;
	treeCtrl->Delete(id);
	
	assert( objectIds[entity.get()] == id );
	objectIds.erase(entity.get());
}

//-----------------------------------//

void ScenePage::onComponentAdded( const ComponentPtr& component )
{
	wxTreeItemId itemId = getTreeIdFromObject( component->getEntity() );
	addComponentToTree(itemId, component);
}

//-----------------------------------//

void ScenePage::onComponentRemoved( const ComponentPtr& component )
{
	wxTreeItemId id = getTreeIdFromObject( component.get() );
	treeCtrl->Delete(id);
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
		if( !animation ) continue;
		
		const String& name = animation->getName();
		if( name.empty() ) continue;

		int id = wxNewId();
		item = menuAnimation->Append(id, name);

		if( !itemFirst ) itemFirst = item;
	}

	if( !itemFirst ) return menuAnimation;
	
	firstAnimationId = itemFirst->GetId();

	Bind( wxEVT_COMMAND_MENU_SELECTED, &ScenePage::onAnimationMenuSelected,
		this, itemFirst->GetId(), item->GetId() );

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
		if( !itemFirst ) itemFirst = item;
	}

	firstAttachmentId = itemFirst->GetId();

	Bind( wxEVT_COMMAND_MENU_SELECTED, &ScenePage::onAttachmentMenuSelected,
		this, itemFirst->GetId(), item->GetId() );

	return menuAttachment;
}

//-----------------------------------//

void ScenePage::populateComponentItemMenu(wxMenu& menu, const ComponentPtr& component)
{
	if( !component ) return;
	Class* klass = component->getType();
	
	menu.SetTitle( klass->name );
	wxMenuItem* remove = menu.Append(ID_MenuSceneComponentRemove, "Remove");
	if( component->is<Transform>() ) remove->Enable(false);

	if( ReflectionIsEqual(klass, ReflectionGetType(Model)) )
	{
		model = RefCast<Model>(component);
		meshHandle = model->getMesh();

		mesh = meshHandle.Resolve();

		if( !mesh || !mesh->isAnimated() )
			return;

		wxMenu* menuAnimation = createMenuAnimation(mesh);
		menu.AppendSubMenu(menuAnimation, "Animation");

		wxMenu* menuAttachment = createMenuAttachment(mesh);
		menu.AppendSubMenu(menuAttachment, "Attachment");
	}
}

//-----------------------------------//

void ScenePage::onComponentMenuSelected(wxCommandEvent& event)
{
	int id = event.GetId();

	switch(id)
	{
	case ID_MenuSceneComponentRemove:
	{
		const ComponentPtr& component = weakComponent;
		const EntityPtr& entity = component->getEntity();

		EntityOperation* entityOperation;
		entityOperation = createEntityOperation("Component removed");
		entityOperation->type = EntityOperation::ComponentRemoved;
		entityOperation->entity = entity;
		entityOperation->component = component;
		entityOperation->redo();

		UndoManager* undoManager = GetEditor().getDocument()->getUndoManager();
		undoManager->registerOperation(entityOperation);
		break;
	}
	default:
		onAnimationMenuSelected(event);
	}
}

//-----------------------------------//

void ScenePage::onAnimationMenuSelected(wxCommandEvent& event)
{
	int id = event.GetId();
	uint ind = id - firstAnimationId;

	const std::vector<AnimationPtr>& anims = mesh->getAnimations();

	if( ind >= anims.size() )
	{
		LogWarn("Invalid animation selected");
		return;
	}
	
	Animation* animation = anims[ind].get();
	model->setAnimation( animation );
	
	event.Skip();
}

//-----------------------------------//

void ScenePage::onAttachmentMenuSelected(wxCommandEvent& event)
{
	int id = event.GetId();
	uint ind = id - firstAttachmentId;
	
	const SkeletonPtr& skeleton = mesh->getSkeleton();
	const std::vector<BonePtr>& bones = skeleton->getBones();

	if( ind >= bones.size() ) return;
	BonePtr bone = bones[ind];

	String name = "Attachment" + StringFromNumber(entityCounter++);
	
	Entity* entity( EntityCreate( AllocatorGetHeap() ) );
	entity->setName(name);
	entity->addTransform();

	Model* model = AllocateHeap(Model);
	model->setMesh(meshHandle);

	entity->addComponent(model);
	
	ScenePtr scene = weakScene;
	scene->entities.add( entity );

	model->setAttachment( bone->name, entity );

	event.Skip();
}

//-----------------------------------//

void ScenePage::onItemMenu(wxTreeEvent& event)
{
	menuItemId = event.GetItem();
	treeCtrl->SelectItem(menuItemId);

	const EntityPtr& entity = getEntityFromTreeId( menuItemId );
	const ComponentPtr& component = getComponentFromTreeId( menuItemId );

	wxMenu menu;
	currentMenu = &menu;

	if( entity )
	{
		weakEntity = entity;
		menu.SetTitle("Entity");
		populateEntityItemMenu(menu, entity.get());
	}
	else
	{
		weakComponent = component;
		menu.SetTitle("Component");
		menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &ScenePage::onComponentMenuSelected, this);
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

	const ScenePtr& scene = weakScene;
	const EntityPtr& node = weakEntity;

	if( !scene ) return;

	switch(id)
	{
	case ID_MenuSceneEntityVisible:
	{
		if( !node ) return;
		node->setVisible( !node->isVisible() );
		break;
	}
	//-----------------------------------//
	case ID_MenuSceneEntityDuplicate:
	{
		if( !node ) return;
		#pragma TODO(Add object cloning)
		break;
	}
	//-----------------------------------//
	case ID_MenuSceneEntityWireframe:
	{
		if( !node ) return;

		PrimitiveRasterMode::Enum mode = event.IsChecked() ? PrimitiveRasterMode::Wireframe : PrimitiveRasterMode::Solid;
		const std::vector<GeometryPtr>& geometries = node->getGeometry();

		for( size_t i = 0; i < geometries.size(); i++ )
		{
			Geometry* geo = geometries[i].get();
			const RenderablesVector& rends = geo->getRenderables();
			
			for( size_t j = 0; j < rends.size(); j++ )
			{
				Renderable* renderable = rends[i].get();
				renderable->setPrimitiveRasterMode( mode );
			}
		}
		break;
	}
	//-----------------------------------//
	case ID_MenuSceneEntityTerrain:
	{
		String name("Terrain"+StringFromNumber(entityCounter++));
		
		#pragma TODO("Terrain needs to be added as an operation")

		TerrainPtr terrain = AllocateThis(Terrain, name);
		scene->entities.add( terrain );

		terrain->addCell(0, 0);
		break;
	}
	default:
	{
		onComponentAdd( event );
		break;
	} }

	event.Skip();
}

//-----------------------------------//

void ScenePage::onComponentAdd(wxCommandEvent& event )
{
	Entity* entity = weakEntity.get();
	
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

	EntityOperation* entityOperation;
	entityOperation = createEntityOperation("Component added");
	entityOperation->type = EntityOperation::ComponentAdded;
	entityOperation->entity = entity;
	entityOperation->component = component;
	entityOperation->redo();

	UndoManager* undoManager = GetEditor().getDocument()->getUndoManager();
	undoManager->registerOperation(entityOperation);
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
	entity->setName( String( label.c_str() ) );
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