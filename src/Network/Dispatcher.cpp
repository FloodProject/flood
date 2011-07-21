/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Network/Dispatcher.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/PluginManager.h"

#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Message.h"
#include "Network/MessageHandlers.h"
#include "Network/Session.h"
#include "Network/SessionManager.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Dispatcher::Dispatcher()
	: isServer(false)
{
	sessions = Allocate(SessionManager, AllocatorGetThis());
	handlers = Allocate(MessageHandlers, AllocatorGetThis());
}

//-----------------------------------//

void Dispatcher::initClient(HostClient* host)
{
	isServer = false;

	//host->onConnected.Connect(this, &Dispatcher::handleConnect);
	//host->onDisconnected.Connect(this, &Dispatcher::handleDisconnect);
	//host->onMessage.Connect(this, &Dispatcher::handleMessage);
}

//-----------------------------------//

void Dispatcher::initServer(HostServer* host)
{
	isServer = true;

	host->onClientConnected.Connect(this, &Dispatcher::handleConnect);
	host->onClientDisconnected.Connect(this, &Dispatcher::handleDisconnect);
	host->onClientMessage.Connect(this, &Dispatcher::handleMessage);
}

//-----------------------------------//

Dispatcher::~Dispatcher()
{
	Deallocate(plugins);
	Deallocate(sessions);
	Deallocate(handlers);
}

//-----------------------------------//

void Dispatcher::initPlugins(Class* klass)
{
	plugins = Allocate(PluginManager, AllocatorGetThis());
	plugins->onPluginEnableEvent.Connect(this, &Dispatcher::handlePluginEnable);
	plugins->onPluginDisableEvent.Connect(this, &Dispatcher::handlePluginDisable);

	std::vector<Plugin*> found;
	plugins->scanPlugins(klass, found);
	plugins->sortPlugins(found);
	plugins->registerPlugins(found);
}

//-----------------------------------//

void Dispatcher::handlePluginEnable(Plugin* plugin)
{
	MessagePlugin* messagePlugin = (MessagePlugin*) plugin;

	const MessagesTable& messagesTable = messagePlugin->getMessagesTable();
	Enum* messagesEnum = messagePlugin->getMessagesEnum();

	MessageDirection::Enum direction = isServer ?
		MessageDirection::ClientToServer :
		MessageDirection::ServerToClient;

	for(size_t i = 0; i < messagesTable.size(); i++ )
	{
		MessageMapping mapping = messagesTable[i];

		if(mapping.direction != direction)
			continue;

		mapping.plugin = messagePlugin;

		handlers->addHandler(mapping);

		const char* name = EnumGetValueName(messagesEnum, mapping.id);
		LogDebug("Registering message type: %s", name);
	}
}

//-----------------------------------//

void Dispatcher::handlePluginDisable(Plugin* plugin)
{
	MessagePlugin* messagePlugin = (MessagePlugin*) plugin;

	const MessagesTable& messagesTable = messagePlugin->getMessagesTable();
	Enum* messagesEnum = messagePlugin->getMessagesEnum();

	for(size_t i = 0; i < messagesTable.size(); i++ )
	{
		const MessageMapping& mapping = messagesTable[i];
		handlers->removeHandler(mapping);
	}
}

//-----------------------------------//

void Dispatcher::handleConnect(const PeerPtr& peer)
{
	SessionPtr session = Allocate(Session, AllocatorGetThis());
	session->setPeer(peer);
	sessions->addSession(session);
}

//-----------------------------------//

void Dispatcher::handleDisconnect(const PeerPtr& peer)
{
	const SessionPtr& session = sessions->getSession(peer);
	sessions->removeSession(session);
}

//-----------------------------------//

void Dispatcher::handleMessage(const PeerPtr& peer, const MessagePtr& message)
{
	const SessionPtr& session = sessions->getSession(peer);

}

//-----------------------------------//

NAMESPACE_CORE_END