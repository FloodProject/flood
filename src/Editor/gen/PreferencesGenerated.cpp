///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Editor/API.h"

#include "PreferencesGenerated.h"

///////////////////////////////////////////////////////////////////////////

Bindings::Bindings( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );
	
	m_panelBindings = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2 = new wxStaticText( m_panelBindings, wxID_ANY, wxT("Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer7->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );
	
	m_treeCtrl1 = new wxTreeCtrl( m_panelBindings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	bSizer7->Add( m_treeCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	bSizer5->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	bSizer8->SetMinSize( wxSize( 160,-1 ) ); 
	m_staticText3 = new wxStaticText( m_panelBindings, wxID_ANY, wxT("Current Shortcuts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizer8->Add( m_staticText3, 0, wxALL|wxEXPAND, 5 );
	
	m_listBox1 = new wxListBox( m_panelBindings, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer8->Add( m_listBox1, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( m_panelBindings, wxID_ANY, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->Enable( false );
	
	bSizer9->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( m_panelBindings, wxID_ANY, wxT("Remove All"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_button2, 0, wxALL, 5 );
	
	bSizer8->Add( bSizer9, 0, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( m_panelBindings, wxID_ANY, wxT("New"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer8->Add( m_staticText4, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrl2 = new wxTextCtrl( m_panelBindings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer8->Add( m_textCtrl2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( m_panelBindings, wxID_ANY, wxT("Currently Assigned"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer8->Add( m_staticText5, 0, wxALL|wxEXPAND, 5 );
	
	m_textCtrl3 = new wxTextCtrl( m_panelBindings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textCtrl3->Enable( false );
	
	bSizer8->Add( m_textCtrl3, 0, wxALL|wxEXPAND, 5 );
	
	m_button3 = new wxButton( m_panelBindings, wxID_ANY, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button3->Enable( false );
	
	bSizer8->Add( m_button3, 0, wxALL|wxEXPAND, 5 );
	
	bSizer5->Add( bSizer8, 0, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( m_panelBindings, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer6->Add( m_staticText1, 0, wxALL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( m_panelBindings, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,48 ), wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY );
	m_textCtrl1->Enable( false );
	
	bSizer6->Add( m_textCtrl1, 1, wxALL|wxEXPAND, 5 );
	
	bSizer4->Add( bSizer6, 0, wxEXPAND, 5 );
	
	m_panelBindings->SetSizer( bSizer4 );
	m_panelBindings->Layout();
	bSizer4->Fit( m_panelBindings );
	bSizer15->Add( m_panelBindings, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer15 );
	this->Layout();
}

Bindings::~Bindings()
{
}

Plugins::Plugins( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_listPlugins = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer11->Add( m_listPlugins, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonPluginEnable = new wxButton( this, wxID_ANY, wxT("Disable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonPluginEnable->Enable( false );
	
	bSizer19->Add( m_buttonPluginEnable, 0, wxALL, 5 );
	
	m_buttonPluginUninstall = new wxButton( this, wxID_ANY, wxT("Uninstall"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonPluginUninstall->Enable( false );
	
	bSizer19->Add( m_buttonPluginUninstall, 1, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer19->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_buttonPluginCheckUpdates = new wxButton( this, wxID_ANY, wxT("Check Updates"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_buttonPluginCheckUpdates, 1, wxALL, 5 );
	
	m_buttonPluginInstall = new wxButton( this, wxID_ANY, wxT("Install..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer19->Add( m_buttonPluginInstall, 1, wxALL, 5 );
	
	bSizer11->Add( bSizer19, 0, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer11, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer61->Add( m_staticText11, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );
	
	m_textPluginDescription = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,48 ), wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY );
	bSizer61->Add( m_textPluginDescription, 1, wxALL|wxEXPAND, 5 );
	
	bSizer18->Add( bSizer61, 0, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer18, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect EventManager
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Plugins::OnPluginInit ) );
	m_listPlugins->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( Plugins::OnPluginSelected ), NULL, this );
	m_buttonPluginEnable->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginEnable ), NULL, this );
	m_buttonPluginCheckUpdates->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginCheckUpdates ), NULL, this );
	m_buttonPluginInstall->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginInstall ), NULL, this );
}

Plugins::~Plugins()
{
	// Disconnect EventManager
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Plugins::OnPluginInit ) );
	m_listPlugins->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( Plugins::OnPluginSelected ), NULL, this );
	m_buttonPluginEnable->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginEnable ), NULL, this );
	m_buttonPluginCheckUpdates->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginCheckUpdates ), NULL, this );
	m_buttonPluginInstall->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginInstall ), NULL, this );
	
}

Resources::Resources( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Thumbnails") ), wxVERTICAL );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Cache Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer23->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_dirPicker1 = new wxDirPickerCtrl( this, wxID_ANY, wxT("C:\\Users\\triton\\.netbeans-registration"), wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	bSizer23->Add( m_dirPicker1, 1, wxALL, 5 );
	
	sbSizer1->Add( bSizer23, 0, wxEXPAND|wxALL, 5 );
	
	bSizer22->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
}

Resources::~Resources()
{
}

Renderers::Renderers( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Default Renderer:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer20->Add( m_staticText8, 0, wxALL, 5 );
	
	wxArrayString m_choice1Choices;
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1Choices, 0 );
	m_choice1->SetSelection( 0 );
	bSizer20->Add( m_choice1, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Renderers:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer20->Add( m_staticText10, 0, wxALL, 5 );
	
	m_listCtrl2 = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	bSizer20->Add( m_listCtrl2, 1, wxALL|wxEXPAND, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Extensions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer20->Add( m_staticText9, 0, wxALL, 5 );
	
	m_listBox3 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,120 ), 0, NULL, 0 ); 
	bSizer20->Add( m_listBox3, 0, wxALL|wxEXPAND, 5 );
	
	bSizer19->Add( bSizer20, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer19 );
	this->Layout();
}

Renderers::~Renderers()
{
}

ResourcesFrame::ResourcesFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter2 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( ResourcesFrame::m_splitter2OnIdle ), NULL, this );
	
	m_panel2 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxVERTICAL );
	
	m_resourceGroups = new wxTreeCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT|wxTR_ROW_LINES );
	bSizer20->Add( m_resourceGroups, 1, wxEXPAND|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	m_panel2->SetSizer( bSizer20 );
	m_panel2->Layout();
	bSizer20->Fit( m_panel2 );
	m_panel3 = new wxPanel( m_splitter2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_searchCtrl = new wxSearchCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	m_searchCtrl->ShowSearchButton( true );
	#endif
	m_searchCtrl->ShowCancelButton( false );
	bSizer21->Add( m_searchCtrl, 1, wxBOTTOM|wxRIGHT|wxEXPAND, 5 );
	
	m_detailSlider = new wxSlider( m_panel3, wxID_ANY, 64, 0, 256, wxDefaultPosition, wxSize( 60,-1 ), wxSL_HORIZONTAL );
	bSizer21->Add( m_detailSlider, 0, 0, 5 );
	
	bSizer18->Add( bSizer21, 0, wxEXPAND|wxTOP, 5 );
	
	m_resourceList = new wxListCtrl( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	bSizer18->Add( m_resourceList, 1, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_gauge1 = new wxGauge( m_panel3, wxID_ANY, 100, wxDefaultPosition, wxSize( -1,8 ), wxGA_HORIZONTAL );
	bSizer18->Add( m_gauge1, 0, wxEXPAND|wxBOTTOM|wxRIGHT, 5 );
	
	m_panel3->SetSizer( bSizer18 );
	m_panel3->Layout();
	bSizer18->Fit( m_panel3 );
	m_splitter2->SplitVertically( m_panel2, m_panel3, 121 );
	bSizer16->Add( m_splitter2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect EventManager
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ResourcesFrame::OnClose ) );
	m_resourceGroups->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourcesFrame::onResourceGroupChanged ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_resourceList->Connect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( ResourcesFrame::OnListBeginDrag ), NULL, this );
	m_resourceList->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ResourcesFrame::onResourceListActivated ), NULL, this );
	m_resourceList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ResourcesFrame::onResourceListSelection ), NULL, this );
}

ResourcesFrame::~ResourcesFrame()
{
	// Disconnect EventManager
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ResourcesFrame::OnClose ) );
	m_resourceGroups->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourcesFrame::onResourceGroupChanged ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_detailSlider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ResourcesFrame::onResourceSliderScroll ), NULL, this );
	m_resourceList->Disconnect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( ResourcesFrame::OnListBeginDrag ), NULL, this );
	m_resourceList->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( ResourcesFrame::onResourceListActivated ), NULL, this );
	m_resourceList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ResourcesFrame::onResourceListSelection ), NULL, this );
	
}

ServerFrame::ServerFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookServer = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelStatus = new wxPanel( m_notebookServer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer251;
	bSizer251 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panelStatus, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxVERTICAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText11 = new wxStaticText( m_panelStatus, wxID_ANY, wxT("Host:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textHost = new wxTextCtrl( m_panelStatus, wxID_ANY, wxT("localhost"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textHost, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText12 = new wxStaticText( m_panelStatus, wxID_ANY, wxT("Port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer1->Add( m_staticText12, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textPort = new wxTextCtrl( m_panelStatus, wxID_ANY, wxT("9999"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textPort->SetValidator( wxTextValidator( wxFILTER_NUMERIC, &port ) );
	
	fgSizer1->Add( m_textPort, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText16 = new wxStaticText( m_panelStatus, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer1->Add( m_staticText16, 0, wxALL, 5 );
	
	m_textName = new wxTextCtrl( m_panelStatus, wxID_ANY, wxT("triton"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_textName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText15 = new wxStaticText( m_panelStatus, wxID_ANY, wxT("Authentication:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer1->Add( m_staticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString m_choiceAuthChoices[] = { wxT("None"), wxT("Password"), wxT("Certificate") };
	int m_choiceAuthNChoices = sizeof( m_choiceAuthChoices ) / sizeof( wxString );
	m_choiceAuth = new wxChoice( m_panelStatus, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceAuthNChoices, m_choiceAuthChoices, 0 );
	m_choiceAuth->SetSelection( 0 );
	fgSizer1->Add( m_choiceAuth, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	sbSizer2->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	bSizer251->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( m_panelStatus, wxID_ANY, wxT("Status:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	bSizer26->Add( m_staticText13, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_textStatus = new wxStaticText( m_panelStatus, wxID_ANY, wxT("Disconnected"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textStatus->Wrap( -1 );
	bSizer26->Add( m_textStatus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer26->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_buttonConnect = new wxButton( m_panelStatus, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_buttonConnect, 0, wxALL|wxEXPAND, 5 );
	
	bSizer251->Add( bSizer26, 0, wxEXPAND, 5 );
	
	m_panelStatus->SetSizer( bSizer251 );
	m_panelStatus->Layout();
	bSizer251->Fit( m_panelStatus );
	m_notebookServer->AddPage( m_panelStatus, wxT("Status"), true );
	m_panelChat = new wxPanel( m_notebookServer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textChat = new wxTextCtrl( m_panelChat, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer24->Add( m_textChat, 1, wxALL|wxEXPAND, 5 );
	
	m_listBox3 = new wxListBox( m_panelChat, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer24->Add( m_listBox3, 0, wxALL|wxEXPAND, 5 );
	
	bSizer25->Add( bSizer24, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textMessage = new wxTextCtrl( m_panelChat, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL|wxTE_PROCESS_ENTER );
	bSizer23->Add( m_textMessage, 1, wxALL, 5 );
	
	m_button10 = new wxButton( m_panelChat, wxID_ANY, wxT("Send"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( m_button10, 0, wxALL, 5 );
	
	bSizer25->Add( bSizer23, 0, wxEXPAND, 5 );
	
	m_panelChat->SetSizer( bSizer25 );
	m_panelChat->Layout();
	bSizer25->Fit( m_panelChat );
	m_notebookServer->AddPage( m_panelChat, wxT("Chat"), false );
	
	bSizer22->Add( m_notebookServer, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect EventManager
	m_buttonConnect->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServerFrame::onConnectButtonClick ), NULL, this );
	m_textMessage->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ServerFrame::onChatSendButtonClick ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServerFrame::onChatSendButtonClick ), NULL, this );
}

ServerFrame::~ServerFrame()
{
	// Disconnect EventManager
	m_buttonConnect->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServerFrame::onConnectButtonClick ), NULL, this );
	m_textMessage->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ServerFrame::onChatSendButtonClick ), NULL, this );
	m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServerFrame::onChatSendButtonClick ), NULL, this );
	
}
