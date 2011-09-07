/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

#ifdef ENABLE_PLUGIN_SERVER

#include "ServerPlugin.h"
#include "Network/Host.h"
#include "Network/Message.h"
#include "Network/Dispatcher.h"
#include "Network/SessionManager.h"
#include "Network/MessageHandler.h"
#include "Protocol/UserMessages.h"
#include "Editor.h"
#include "Settings.h"
#include "ServerPane.h"
#include "Core/Utilities.h"
#include "EventManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ServerPlugin, EditorPlugin)
REFLECT_CLASS_END()

enum ServerMenuIDs
{
	ServerConnectId = wxID_HIGHEST + 1,
};

//-----------------------------------//

ServerPlugin::ServerPlugin()
	: host(nullptr)
	, dispatcher(nullptr)
	, networkThread(nullptr)
	, pane(nullptr)
	, isDisconnecting(false)
{
}

//-----------------------------------//

PluginMetadata ServerPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Server";
	metadata.description = "Handles connecting to remote hosts.";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.priority = 900;
	metadata.startEnabled = false;

	return metadata;
}

//-----------------------------------//

void ServerPlugin::onPluginEnable()
{
	EditorFrame* editor = &GetEditor();

	wxMenu* menuServer = editor->menuServer;

	wxMenuItem* itemConnect = menuServer->Append(ServerConnectId, "Connect...");
	editor->Bind(wxEVT_COMMAND_MENU_SELECTED, &ServerPlugin::onConnectClick, this, itemConnect->GetId());

	pane = new ServerPane(editor);
	pane->Show();

	HostConnectionDetails details;
	details.address = HostAddress;
	details.port = HostPort;

	initClient(details);
}

//-----------------------------------//

void ServerPlugin::onPluginDisable()
{
	PluginMetadata metadata = getMetadata();

	disconnect();
	deinitClient();

	delete pane;
}

//-----------------------------------//

void ServerPlugin::onPluginUpdate()
{
	if(isDisconnecting)
	{
		//deinitClient();
		isDisconnecting = false;
	}

	if( !host ) return;
	
	HostState::Enum state = host->getState();

	switch(state)
	{
	case HostState::Connected:
		auth();
		break;
	}

	host->processEvents(1);
	dispatcher->processMessage();
}

//-----------------------------------//

bool ServerPlugin::initClient(const HostConnectionDetails& details)
{
	host = Allocate(HostClient, AllocatorGetThis());
	
	dispatcher = Allocate(Dispatcher, AllocatorGetThis());
	dispatcher->initClient(host);

	SessionManager* sm = dispatcher->getSessionManager();
	sm->onSessionAdded.Connect(this, &ServerPlugin::handleSessionAdded);
	sm->onSessionRemoved.Connect(this, &ServerPlugin::handleSessionRemoved);

	connect(details);

#ifdef NETWORK_THREAD
	networkThread = ThreadCreate( AllocatorGetThis() );

	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return false;
	}

	ThreadFunction processFunction;
	processFunction.Bind(this, &ServerPlugin::processMessages);

	ThreadStart(networkThread, processFunction, this);
	ThreadSetName(networkThread, "Networking");
#endif

	return true;
}

//-----------------------------------//

void ServerPlugin::deinitClient()
{
#ifdef NETWORK_THREAD
	networkThread->IsRunning = false;
	ThreadJoin(networkThread);
	ThreadDestroy(networkThread);
	networkThread = nullptr;
#endif

	Deallocate(host);
	host = nullptr;

	Deallocate(dispatcher);
	dispatcher = nullptr;
}

//-----------------------------------//
#ifdef NETWORK_THREAD
void ServerPlugin::processMessages(Thread* thread, void* userdata)
{
	while(thread->IsRunning)
	{
		if( !host || !host->hasContext() )
		{
			SystemSleep(100);
			continue;
		}

		HostState::Enum state = host->getState();

		switch(state)
		{
		case HostState::Disconnected:
			continue;
			break;
		case HostState::Connected:
			auth();
			break;
		}

		host->processEvents(100);
	}
}
#endif
//-----------------------------------//

void ServerPlugin::connect(const HostConnectionDetails& details)
{
	LogInfo("Attempting to connect to server...");

	if( !host->connect(details) )
	{
		LogError("Error connecting to server...");
		return;
	}
}

//-----------------------------------//

void ServerPlugin::auth()
{
	UserAuthMessage auth;
	auth.name = pane->getUserName();
	auth.type = UserAuthType::None;

	MessagePtr message = MessageCreate(UserMessageIds::UserAuth);
	message->write(&auth);
			
	host->setState(HostState::Authenticating);
	host->getPeer()->queueMessage(message, 0);
}

//-----------------------------------//

void ServerPlugin::disconnect()
{
	if( !host ) return;

	const PeerPtr& peer = host->getPeer();
	if( !peer ) return;
	
	peer->disconnect();
}

//-----------------------------------//

bool ServerPlugin::isConnected()
{
	return host && (host->getState() != HostState::Disconnected);
}

//-----------------------------------//

void ServerPlugin::onConnectClick(wxCommandEvent& event)
{
}

//-----------------------------------//

void ServerPlugin::handleSessionAdded(const SessionPtr& session)
{
	const PeerPtr& peer = session->getPeer();
	LogInfo("Connected to server at: %s", peer->getHostIP().c_str());
	pane->onClientConnect(peer);
	GetEditor().getEventManager()->onServerConnect(session);
}

//-----------------------------------//

void ServerPlugin::handleSessionRemoved(const SessionPtr& session)
{
	const PeerPtr& peer = session->getPeer();
	LogInfo("Disconnected from server at: %s", peer->getHostIP().c_str());
	pane->onClientDisconnect(peer);
	isDisconnecting = true;
	GetEditor().getEventManager()->onServerDisconnect(session);
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif