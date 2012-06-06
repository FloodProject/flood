/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Preferences.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

void EditorFrame::createMenus()
{
	menuFile = new wxMenu;

	//-----------------------------------//

	menuEdit = new wxMenu;
	wxMenuItem* editPreferences = menuEdit->Append(wxID_ANY, "&Preferences...");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnPreferences, this, editPreferences->GetId());

	//-----------------------------------//

	menuProject = new wxMenu;

	//-----------------------------------//

	menuTools = new wxMenu;

	//-----------------------------------//

	menuHelp = new wxMenu;
	wxMenuItem* aboutItem = menuHelp->Append(wxID_ABOUT, "&About...\tF1");
	wxMenuItem* aboutWxItem = menuHelp->Append(wxID_ANY, "&About wxWidgets...");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnAbout, this, aboutItem->GetId());
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnAboutWx, this, aboutWxItem->GetId());
	
	//-----------------------------------//

	menuWindows = new wxMenu;
	Bind(wxEVT_UPDATE_UI, &EditorFrame::OnPanelsMenuUpdate, this);
	Bind(wxEVT_MENU_OPEN, &EditorFrame::OnMenuOpenEvent, this);
	
	//-----------------------------------//

	menuSettings = new wxMenu;
	wxMenuItem* renderItem = menuSettings->AppendCheckItem(wxID_ANY, "&Fixed Pipeline");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnSettingsRender, this, renderItem->GetId());
	Bind(wxEVT_UPDATE_UI, &EditorFrame::OnSettingsRenderUpdate, this, renderItem->GetId());

	//-----------------------------------//

#ifdef ENABLE_PLUGIN_SERVER
	menuServer = new wxMenu;
#endif

	//-----------------------------------//

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuEdit, "&Edit");
	menuBar->Append(menuSettings, "&Settings");
	menuBar->Append(menuTools, "&Tools");
	menuBar->Append(menuWindows, "&Windows");
#if ENABLE_PLUGIN_SERVER
	menuBar->Append(menuServer, "&Server");
#endif
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);
}

//-----------------------------------//

void EditorFrame::createLastUI()
{
	wxMenuItem* quitItem = menuFile->Append(wxID_EXIT, "E&xit\tCtrl-F4", "Quit this program");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnMenuExitEvent, this, quitItem->GetId());
}

//-----------------------------------//

void EditorFrame::OnMenuOpenEvent(wxMenuEvent& event)
{
	if( event.GetMenu() != menuWindows )
		return;

	const wxMenuItemList& menus = menuWindows->GetMenuItems();
	if( !menus.IsEmpty() ) return;

	wxAuiManager* aui = getAUI();
	wxAuiPaneInfoArray& panes = aui->GetAllPanes();

	for( size_t i = 0; i < panes.size(); i++ )
	{
		wxAuiPaneInfo& pane = panes[i];
		
		if( pane.caption.IsEmpty() )
			continue;
		
		wxMenuItem* item = menuWindows->AppendCheckItem(wxID_ANY, pane.caption);
		item->Check( pane.IsShown() );

		Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnPanelsMenuEvent, this, item->GetId());
	}
}

//-----------------------------------//

void EditorFrame::OnPanelsMenuEvent(wxCommandEvent& event)
{
	wxMenuItem* item = menuWindows->FindItem( event.GetId() );
	if( !item ) return;

	wxAuiPaneInfo& pane = paneCtrl->GetPane( item->GetItemLabelText() );
	if( !pane.IsOk() ) return;

	pane.Show( !pane.IsShown() );
	paneCtrl->Update();
}

//-----------------------------------//

void EditorFrame::OnPanelsMenuUpdate(wxUpdateUIEvent& event)
{
	
}

//-----------------------------------//

void EditorFrame::OnSettingsRender(wxCommandEvent& event)
{
	RenderPipeline::Enum pipeline = event.IsChecked() ?
		RenderPipeline::Fixed : RenderPipeline::ShaderForward;

	RenderDevice* render = GetEngine()->getRenderDevice();
	render->setPipeline(pipeline);
}

//-----------------------------------//

void EditorFrame::OnSettingsRenderUpdate(wxUpdateUIEvent& event)
{
	RenderDevice* render = GetEngine()->getRenderDevice();
	event.Check( render->getPipeline() == RenderPipeline::Fixed );
}

//-----------------------------------//

void EditorFrame::OnMenuExitEvent(wxCommandEvent& WXUNUSED(event))
{
	Close();
}

//-----------------------------------//

void EditorFrame::OnPreferences(wxCommandEvent& event)
{
	Preferences* pref = new Preferences(this);
	
	if( pref->ShowModal() == wxID_OK )
	{
		#pragma TODO("Handle saving the user preferences")
	}
}

//-----------------------------------//

static const char* s_AboutText =
	"This software is © 2008-201x João Matos and the rest of the team.\n\n"
	VAPOR_EDITOR_NAME " uses some free software packages: wxWidgets (wxWidgets.org),"
	" Mono (mono-project.com),\nBullet (bulletphysics.com), zlib (zlib.org)"
	" and others.\n\nCheck the documentation provided with the software"
	" for more details.";

const char* s_SiteText = "http://www.vapor3d.org";

void EditorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* about = new wxDialog(this, wxID_ANY, "About " VAPOR_EDITOR_NAME);

	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxPanel* m_panel1 = new wxPanel( about );
	wxBitmap aboutIcon = wxMEMORY_BITMAP(::about);
	wxStaticBitmap* m_bitmap1 = new wxStaticBitmap( m_panel1, wxID_ANY, aboutIcon);
	bSizer2->Add( m_bitmap1, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 0 );

	wxString aboutText(s_AboutText);
	wxStaticText* m_staticText2 = new wxStaticText( m_panel1, wxID_ANY, aboutText );
	m_staticText2->Wrap( -1 );
	bSizer2->Add( m_staticText2, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	wxStaticLine* m_staticline1 = new wxStaticLine( m_panel1, wxID_ANY );
	bSizer3->Add( m_staticline1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	wxHyperlinkCtrl* m_hyperlink1 = new wxHyperlinkCtrl( m_panel1, wxID_ANY, s_SiteText, s_SiteText );
	bSizer3->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticLine* m_staticline2 = new wxStaticLine( m_panel1, wxID_ANY );
	bSizer3->Add( m_staticline2, 1, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxStdDialogButtonSizer* btnSizer = new wxStdDialogButtonSizer();
	wxButton* buttonOK = new wxButton( m_panel1, wxID_OK );
	btnSizer->AddButton( buttonOK );
	bSizer2->Add(btnSizer);

	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();

	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );
	
	about->SetSizer( bSizer1 );
	about->Layout();
	bSizer1->Fit( about );

	about->ShowModal();
}

//-----------------------------------//

void EditorFrame::OnAboutWx(wxCommandEvent& WXUNUSED(event))
{
	wxInfoMessageBox(this);
}

//-----------------------------------//

NAMESPACE_EDITOR_END
