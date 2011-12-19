/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ScenePlugin.h"
#include "ScenePane.h"
#include "SceneDocument.h"
#include "Plugins/Property/PropertyPlugin.h"
#include "Plugins/Property/PropertyPage.h"
#include "Plugins/Property/PropertyGrid.h"
#include "Plugins/Networking/ServerPlugin.h"
#include "Protocol/ReplicaMessages.h"
#include "Protocol/ReplicaContext.h"
#include "Network/Session.h"
#include "Network/Host.h"
#include "Network/MessageDispatcher.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "EditorTags.h"
#include "EventManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ScenePlugin, EditorPlugin)
REFLECT_CLASS_END()

//-----------------------------------//

ScenePlugin::ScenePlugin() : scenePage(nullptr)
{
}

//-----------------------------------//

PluginMetadata ScenePlugin::getMetadata()
{
	static PluginMetadata metadata;
	
	metadata.name = "Scene";
	metadata.description = "Provides a page with the scene contents.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 500;

	return metadata;
}

//-----------------------------------//

void ScenePlugin::onPluginEnable()
{
	scenePage = new ScenePage(editor);
	scenePage->SetMinSize( wxSize(200, 200) );

	wxBitmap icon = wxMEMORY_BITMAP(sitemap_color);

	wxAuiPaneInfo pane;
	pane.Caption("Hierarchy").Name("Hierarchy").Right().Hide().Dock().Icon(icon);
	pane.MinSize( scenePage->GetMinSize() );
	pane.dock_proportion = 1;

	editor->getAUI()->AddPane(scenePage, pane);
	editor->getAUI()->Update();

	wxAuiToolBar* toolbarCtrl = GetEditor().getToolbar();
	toolbarCtrl->Bind(wxEVT_COMMAND_MENU_SELECTED, &ScenePlugin::onPlayCommand, this, Toolbar_TooglePlay);

	// Subscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->addEventListener(this);

#ifdef ENABLE_PLUGIN_PROPERTY
	PropertyPlugin* pp = GetPlugin<PropertyPlugin>();
	pp->propertyPage->onClassFieldChanged.Bind(this, &ScenePlugin::onSceneClassFieldUpdate);
#endif

#ifndef NO_NETWORK
	ReplicaMessageHandler* rmp = GetMessageHandler<ReplicaMessageHandler>();
	rmp->onReplicaContextCreate.Connect(this, &ScenePlugin::onReplicaContextCreate);
	rmp->onReplicaObjectCreate.Connect(this, &ScenePlugin::onReplicaObjectCreate);
#endif
}

//-----------------------------------//

void ScenePlugin::onPluginDisable()
{
	//removePage( scenePage );
	editor->getAUI()->DetachPane(scenePage);
	//editor->getAUI()->Update();

	Deallocate(scenePage);

	// Unsubscribe as an event listener.
	EventManager* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void ScenePlugin::onPlayCommand(wxCommandEvent& event)
{
	bool switchToPlay = event.IsChecked();
	//switchPlayMode(switchToPlay);

#ifdef ENABLE_PHYSICS_BULLET
	// Toogle the physics simulation state.
	PhysicsManager* physics = GetEngine()->getPhysicsManager();
	
	if( physics )
		physics->setSimulation( switchToPlay );
#endif
}

//-----------------------------------//

void ScenePlugin::onEntitySelect( const EntityPtr& entity )
{
	entity->onComponentAdded.Connect(this, &ScenePlugin::onComponentChanged);
	entity->onComponentRemoved.Connect(this, &ScenePlugin::onComponentChanged);

	setBoundingBoxVisible(entity, true);

	if( ClassInherits(entity->getType(), ReflectionGetType(Group)) )
		return;

	showEntityProperties( entity.get() );
}

//-----------------------------------//

void ScenePlugin::onEntityUnselect( const EntityPtr& entity )
{
	//wxTreeCtrl* treeCtrl = scenePage->getTreeCtrl();
	//treeCtrl->Unselect();

	PropertyPage* propertyPage = GetPlugin<PropertyPlugin>()->propertyPage;

	if( propertyPage->getObject() == entity.get() )
	{
		propertyPage->resetObject( entity.get() );
		propertyPage->update();
	}

	entity->onComponentAdded.Disconnect(this, &ScenePlugin::onComponentChanged);
	entity->onComponentRemoved.Disconnect(this, &ScenePlugin::onComponentChanged);

	setBoundingBoxVisible(entity, false);
}

//-----------------------------------//

void ScenePlugin::setBoundingBoxVisible(const EntityPtr& entity, bool state)
{
	if( !entity ) return;

	Transform* transform = entity->getTransform().get();
	if( !transform ) return;

	transform->setDebugRenderableVisible( state );
}

//-----------------------------------//

void ScenePlugin::onComponentSelect( const ComponentPtr& component )
{
	const EntityPtr& entity = component->getEntity();
	onEntitySelect(entity);
}

//-----------------------------------//

void ScenePlugin::onComponentUnselect( const ComponentPtr& component )
{
	const EntityPtr& entity = component->getEntity();
	onEntityUnselect(entity);
}

//-----------------------------------//

void ScenePlugin::onComponentChanged(const ComponentPtr& component)
{
	const EntityPtr& entity = component->getEntity();
	showEntityProperties( entity.get() );
	//editor->getDocument()->getWindow()->flagRedraw();
}

//-----------------------------------//

void ScenePlugin::onSceneLoad( const ScenePtr& scene )
{
	scenePage->setScene(scene);
}

//-----------------------------------//

void ScenePlugin::onSceneUnload( const ScenePtr& scene )
{
	LogDebug("Scene unloaded, removing items from scene tree");
	scenePage->cleanScene();

	PropertyPage* page = GetPlugin<PropertyPlugin>()->propertyPage;
	page->reset();
}

//-----------------------------------//

void ScenePlugin::setPropertyHandler(Class* klass, const PropertyEvent& event)
{
	propertyHandlers[klass] = event;
}

//-----------------------------------//

void ScenePlugin::removePropertyHandler(Class* klass)
{
	auto it = propertyHandlers.find(klass);

	if( it == propertyHandlers.end() )
		return;

	propertyHandlers.erase(it);
}

//-----------------------------------//

static wxFoldPanel CreateComponentPanel( PropertyPage* page, Component* component )
{
	if( !component ) return wxFoldPanel();

	wxFoldPanel foldPanel = page->AddFoldPanel( component->getType()->name );

	wxImage image = GetIconFromComponent(component->getType());
	foldPanel.SetIcon(&image);

	PropertyGrid* grid = page->createPropertyGrid( foldPanel.GetParent() );
	page->AddFoldPanelWindow(foldPanel, grid, wxSizerFlags().Expand());

	page->appendObjectFields( grid, component->getType(), component );

	return foldPanel;
}

//-----------------------------------//

void ScenePlugin::showEntityProperties( Entity* entity )
{
	PropertyPage* page = GetPlugin<PropertyPlugin>()->propertyPage;

	page->reset();
	page->setObject(entity);

	{
	wxFoldPanel foldPanel = page->AddFoldPanel("Entity");
	foldPanel.GetItem()->GetCaptionBar()->Hide();

	PropertyGrid* grid = page->createPropertyGrid( foldPanel.GetParent() );
	page->AddFoldPanelWindow(foldPanel, grid, wxSizerFlags().Expand());

	// Entity properties.
	page->appendObjectFields( grid, entity->getType(), entity );
	}

	// Transform properties.
	TransformPtr transform = entity->getTransform();

	if( transform )
	{
		wxFoldPanel panel = CreateComponentPanel(page, transform.get());
	}

	// Other components properties.
	const ComponentMap& components = entity->getComponents();
	
	ComponentMap::const_iterator it;
	for( it = components.begin(); it != components.end(); it++ )
	{
		Class* type = it->first;
		const ComponentPtr& component = it->second;

		if( ReflectionIsEqual(type, ReflectionGetType(Transform)) )
			continue;

		wxFoldPanel panel = CreateComponentPanel(page, component.get());

		// Check if there are property handlers for this component.
		auto it = propertyHandlers.find(type);
		if( it == propertyHandlers.end() ) continue;

		PropertyEvent& event = it->second;
		event(page, panel);
	}

#if 0
	wxStaticLine* staticLine = new wxStaticLine(page);
	page->GetSizer()->Add(staticLine, wxSizerFlags().Expand());
#endif

	page->update();
}

//-----------------------------------//

void ScenePlugin::onServerConnect(const SessionPtr&)
{
#ifdef ENABLE_PLUGIN_SERVER
	localId = 0;

	ReplicaContextCreateMessage cc;
	cc.localId = localId;
	cc.classId = ReflectionGetType(Scene)->id;

	MessagePtr msg = MessageCreate(ReplicaMessageIds::ReplicaContextCreate);
	msg->write(&cc);
	GetPlugin<ServerPlugin>()->getHost()->broadcastMessage(msg);
#endif
}

//-----------------------------------//

#ifndef NO_NETWORK

void ScenePlugin::onReplicaContextCreate(ReplicaContext* context, ClassId, ReplicaLocalId id)
{
	if( id != localId ) return;
	context->onReplicaAdded.Bind(this, &ScenePlugin::onReplicaAdded);

	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	document->replicaContext = context;
}

//-----------------------------------//

void ScenePlugin::onReplicaObjectCreate(ReplicaContext* context, ReplicaInstanceId parentId, Object* object)
{
	Class* klass = ClassGetType(object);

	if( ClassInherits(klass, ComponentGetType()) )
	{
		Component* component = (Component*) object;
	
		Entity* entity = (Entity*) context->findInstanceById(parentId)->instance;
		if( !entity ) return;

		entity->addComponent(component);
		ScenePage* page = GetPlugin<ScenePlugin>()->scenePage;
		
		wxTreeItemId id = page->getTreeIdFromObject(entity);
		page->addComponentToTree(id, component);
	}
	else if( ClassInherits(klass, EntityGetType()) )
	{
		Entity* entity = (Entity*) object;

		SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
		document->scene->add(entity);
	}
}

//-----------------------------------//

void ScenePlugin::onReplicaAdded(const ReplicatedObject& obj)
{
}

#endif

//-----------------------------------//

void ScenePlugin::onSceneClassFieldUpdate(const FieldWatchVector& watches)
{
#ifndef NO_NETWORK
	SceneDocument* document = (SceneDocument*) GetEditor().getDocument();
	MessagePtr msg = document->replicaContext->createObjectUpdateMessage(watches);
	GetPlugin<ServerPlugin>()->host->broadcastMessage(msg);
#endif
}

//-----------------------------------//

NAMESPACE_EDITOR_END