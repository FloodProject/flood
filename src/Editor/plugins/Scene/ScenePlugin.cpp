/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Plugins/Scene/ScenePlugin.h"
#include "Plugins/Scene/ScenePane.h"
#include "Plugins/Scene/SceneDocument.h"
#include "Plugins/Property/PropertyPlugin.h"
#include "Plugins/Property/PropertyPage.h"
#include "Plugins/Networking/ServerPlugin.h"
#include "Protocol/ReplicaMessages.h"
#include "Protocol/ReplicaContext.h"
#include "Network/Session.h"
#include "Network/Host.h"
#include "Network/Dispatcher.h"
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
	PluginMetadata metadata;
	
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
	scenePage->SetSize(180, -1);

	wxBitmap icon = wxMEMORY_BITMAP(sitemap_color);

	wxAuiPaneInfo pane;
	pane.Caption("Hierarchy").Name("Hierarchy").Right().Hide().Dock().Icon(icon);
	pane.MinSize( scenePage->GetSize() );

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
	editor->getAUI()->Update();

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
	if( scenePage->sentLastSelectionEvent )
		return;

	wxTreeItemId entityId = scenePage->getTreeIdFromObject( entity.get() );

	if( !entityId.IsOk() )
		return;
}

//-----------------------------------//

void ScenePlugin::onEntityUnselect( const EntityPtr& entity )
{
	if( scenePage->sentLastSelectionEvent )
		return;

	wxTreeCtrl* treeCtrl = scenePage->getTreeCtrl();
	treeCtrl->Unselect();
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