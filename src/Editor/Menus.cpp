/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Editor.h"
#include "EditorIcons.h"

namespace vapor { namespace editor {

//-----------------------------------//

void EditorFrame::createMenus()
{
	fileMenu = new wxMenu;

	//-----------------------------------//

    editMenu = new wxMenu;

	//-----------------------------------//

    toolsMenu = new wxMenu;

	//-----------------------------------//

    helpMenu = new wxMenu;
	wxMenuItem* helpItem = helpMenu->Append(wxID_ABOUT, "&About...\tF1");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnAbout, this, helpItem->GetId());

	//-----------------------------------//

	panelsMenu = new wxMenu;
	Bind(wxEVT_UPDATE_UI, &EditorFrame::OnPanelsMenuUpdate, this);
	Bind(wxEVT_MENU_OPEN, &EditorFrame::OnMenuOpenEvent, this);
	
	//-----------------------------------//

	settingsMenu = new wxMenu;
	wxMenuItem* renderItem = settingsMenu->AppendCheckItem(wxID_ANY, "&Fixed Pipeline");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnSettingsRender, this, renderItem->GetId());
	Bind(wxEVT_UPDATE_UI, &EditorFrame::OnSettingsRenderUpdate, this, renderItem->GetId());

	//-----------------------------------//

    wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(editMenu, "&Edit");
	menuBar->Append(settingsMenu, "&Settings");
	menuBar->Append(toolsMenu, "&Tools");
	menuBar->Append(panelsMenu, "&Panels");
    menuBar->Append(helpMenu, "&Help");

    SetMenuBar(menuBar);
}

//-----------------------------------//

void EditorFrame::createLastUI()
{
	wxMenuItem* quitItem = fileMenu->Append(wxID_ANY, "E&xit\tAlt-X", "Quit this program");
	Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnQuit, this, quitItem->GetId());

	getAUI()->Update();
}

//-----------------------------------//

void EditorFrame::OnMenuOpenEvent(wxMenuEvent& event)
{
	if( event.GetMenu() != panelsMenu )
		return;

	const wxMenuItemList menus = panelsMenu->GetMenuItems();

	if( !menus.IsEmpty() )
		return;
	//{
	//	wxMenuItem* item = (wxMenuItem *) menus.GetFirst();
	//	
	//	if(item)
	//		panelsMenu->Delete(item);
	//}

	wxAuiManager* aui = getAUI();

	wxAuiPaneInfoArray& panes = aui->GetAllPanes();

	for( uint i = 0; i < panes.size(); i++ )
	{
		wxAuiPaneInfo& pane = panes[i];
		
		if( pane.caption.IsEmpty() )
			continue;
		
		wxMenuItem* item = panelsMenu->Append(wxID_ANY, pane.caption);
		//item->Check(pane.IsShown());
		//item->SetBitmap(pane.icon);

		Bind(wxEVT_COMMAND_MENU_SELECTED, &EditorFrame::OnPanelsMenuEvent, this, item->GetId());
	}
}

//-----------------------------------//

void EditorFrame::OnPanelsMenuEvent(wxCommandEvent& event)
{
	wxMenuItem* item = panelsMenu->FindItem( event.GetId() );

	if( !item )
		return;

	wxAuiPaneInfo& pane = auiManager->GetPane( item->GetItemLabelText() );

	if( !pane.IsOk() )
		return;

	pane.Show( !pane.IsShown() );
	//item->Check( pane.IsShown() );

	auiManager->Update();
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

void EditorFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // Forces the frame to close.
    Close(true);
}

//-----------------------------------//

void EditorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxFrame* about = new wxFrame(this, wxID_ANY, "About " VAPOR_EDITOR_NAME,
		wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|
		wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxTAB_TRAVERSAL);

	wxBoxSizer* bSizer1 = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxPanel* m_panel1 = new wxPanel( about );

	wxBitmap aboutIcon = wxMEMORY_BITMAP(::about);
	wxStaticBitmap* m_bitmap1 = new wxStaticBitmap( m_panel1, wxID_ANY, aboutIcon);
	bSizer2->Add( m_bitmap1, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 0 );

	wxString aboutText(
		"This software is © 2009-2010 João Matos and the rest of the team.\n\n"
		VAPOR_EDITOR_NAME " uses some free software packages: wxWidgets (wxWidgets.org),"
		" Lua (lua.org),\nBullet (bulletphysics.com), zlib (zlib.org)"
		" and the list goes on.\n\nCheck the documentation provided with the software"
		" for more details.");

	wxStaticText* m_staticText2 = new wxStaticText( m_panel1, wxID_ANY, aboutText );
	m_staticText2->Wrap( -1 );
	bSizer2->Add( m_staticText2, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticLine* m_staticline1 = new wxStaticLine( m_panel1, wxID_ANY );
	bSizer3->Add( m_staticline1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	wxHyperlinkCtrl* m_hyperlink1 = new wxHyperlinkCtrl( m_panel1, wxID_ANY,
		"vapor3D (http://www.vapor3d.org)", "http://www.vapor3d.org" );
	bSizer3->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	//wxStaticText* m_hyperlink1 = new wxStaticText( m_panel1, wxID_ANY,
	//	"http://www.vapor3d.org" );
	//bSizer3->Add( m_hyperlink1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxStaticLine* m_staticline2 = new wxStaticLine( m_panel1, wxID_ANY );
	bSizer3->Add( m_staticline2, 1, wxALL|wxALIGN_CENTER_VERTICAL, 10 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();

	bSizer2->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND, 5 );
	
	about->SetSizer( bSizer1 );
	about->Layout();
	bSizer1->Fit( about );

	about->Show(true);
}	

//-----------------------------------//

} } // end namespaces
