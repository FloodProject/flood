/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Server/API.h"
#include "Server/ServerPlugin.h"
#include "Protocol/ReplicaMessages.h"

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(ReplicaPlugin)

class ReplicaPlugin : public ServerPlugin, public ReplicaMessageHandler
{
	REFLECT_DECLARE_OBJECT(ReplicaPlugin)

public:

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	void handleReplicaContextCreate(const SessionPtr&, const ReplicaContextCreateMessage&) OVERRIDE;
	void handleReplicaContextRequest(const SessionPtr&, const ReplicaContextRequestMessage&) OVERRIDE;

	void handleReplicaObjectCreate(const SessionPtr&, const ReplicaObjectCreateMessage&) OVERRIDE;
	void handleReplicaObjectUpdate(const SessionPtr&, const MessagePtr&) OVERRIDE;
};

//-----------------------------------//

NAMESPACE_SERVER_END