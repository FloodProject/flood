/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ServerPane.h"
#include "ServerPlugin.h"

#ifdef ENABLE_RESOURCE_BROWSER

#include "Viewframe.h"
#include "EditorIcons.h"
#include "UndoManager.h"
#include "Settings.h"
#include "Editor.h"
#include "Events.h"
#include "Core/Utilities.h"
#include "Network/Host.h"
#include "Network/Message.h"
#include "Protocol/ChatMessages.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

ServerPane::ServerPane( wxWindow* parent )
	: ServerFrame(parent)
	, connect(true)
{
	Bind(wxEVT_CLOSE_WINDOW, &ServerPane::OnClose, this);
}

//-----------------------------------//

void ServerPane::onConnectButtonClick(wxCommandEvent& event)
{
	ServerPlugin* serverPlugin = GetPlugin<ServerPlugin>();

	if(connect)
	{
		uint32 port;
		m_textPort->GetValue().ToULong(&port);

		HostConnectionDetails details;
		details.address = m_textHost->GetValue();
		details.port = port;

		serverPlugin->connect(details);
	}
	else
	{
		serverPlugin->disconnect();
	}
}

//-----------------------------------//

void ServerPane::onChatSendButtonClick(wxCommandEvent& event)
{
	ServerPlugin* serverPlugin = GetPlugin<ServerPlugin>();

	String text = m_textMessage->GetValue();
	
	ChatClientMessage chat;
	chat.text = text;

	MessagePtr message = MessageCreate(ChatMessageIds::ChatClient);
	message->write(&chat);
	
	serverPlugin->host->getPeer()->queueMessage(message, 0);

	m_textMessage->Clear();

	m_textChat->AppendText(text+"\n");
}

//-----------------------------------//

void ServerPane::onClientConnect(const PeerPtr& peer)
{
	m_textStatus->SetLabel("Connected");
	m_buttonConnect->SetLabel("Disconnect");
	connect = false;
}

//-----------------------------------//

void ServerPane::onClientDisconnect(const PeerPtr& peer)
{
	m_textStatus->SetLabel("Disconnected");
	m_buttonConnect->SetLabel("Connect");
	connect = true;
}

//-----------------------------------//

String ServerPane::getUserName()
{
	String name = m_textName->GetValue();
	return name;
}

//-----------------------------------//

void ServerPane::OnClose(wxCloseEvent& event)
{
	if ( !event.CanVeto() ) return;

	Hide();
	event.Veto();
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif