/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "gen\PreferencesGenerated.h"

FWD_DECL_INTRUSIVE(Peer)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;

/**
 * Server pane provides server-related functionality and debugging info.
 */

class ServerPane : public ServerFrame
{
public:

	ServerPane( wxWindow* parent );

	// Gets the user name.
	String getUserName();

	// Host callbacks.
	void onClientConnect(const PeerPtr&);
	void onClientDisconnect(const PeerPtr&);

protected:

	void setupUI();

	// Widget callbacks.
	void onConnectButtonClick(wxCommandEvent& event) OVERRIDE;
	void onChatSendButtonClick(wxCommandEvent& event) OVERRIDE;

	uint32 port;
	bool connect;
};

//-----------------------------------//

NAMESPACE_EDITOR_END