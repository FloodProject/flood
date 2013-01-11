/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_PLUGIN_SERVER

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

	// Event handlers.
	void OnClose(wxCloseEvent& event);

	// Widget callbacks.
	void onConnectButtonClick(wxCommandEvent& event) OVERRIDE;
	void onChatSendButtonClick(wxCommandEvent& event) OVERRIDE;

	uint32 port;
	bool connect;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif