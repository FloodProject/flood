///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 22 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Editor/API.h"

#include "PreferencesGenerated.h"

///////////////////////////////////////////////////////////////////////////
using namespace vapor::gui;

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
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( Plugins::OnPluginInit ) );
	m_listPlugins->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( Plugins::OnPluginSelected ), NULL, this );
	m_buttonPluginEnable->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginEnable ), NULL, this );
	m_buttonPluginCheckUpdates->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginCheckUpdates ), NULL, this );
	m_buttonPluginInstall->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Plugins::OnPluginInstall ), NULL, this );
}

Plugins::~Plugins()
{
	// Disconnect Events
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
	
	bSizer22->Add( sbSizer1, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Resource Loaders") ), wxVERTICAL );
	
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
	
	sbSizer2->Add( bSizer11, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
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
	
	sbSizer2->Add( bSizer1, 0, wxEXPAND, 5 );
	
	bSizer22->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	
	// Connect Events
	m_listPlugins->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( Resources::OnPluginSelected ), NULL, this );
	m_buttonPluginEnable->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Resources::OnPluginEnable ), NULL, this );
	m_buttonPluginCheckUpdates->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Resources::OnPluginCheckUpdates ), NULL, this );
	m_buttonPluginInstall->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Resources::OnPluginInstall ), NULL, this );
}

Resources::~Resources()
{
	// Disconnect Events
	m_listPlugins->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( Resources::OnPluginSelected ), NULL, this );
	m_buttonPluginEnable->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Resources::OnPluginEnable ), NULL, this );
	m_buttonPluginCheckUpdates->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Resources::OnPluginCheckUpdates ), NULL, this );
	m_buttonPluginInstall->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Resources::OnPluginInstall ), NULL, this );
	
}

ResourcesBrowser::ResourcesBrowser( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_splitter2 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter2->Connect( wxEVT_IDLE, wxIdleEventHandler( ResourcesBrowser::m_splitter2OnIdle ), NULL, this );
	
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
	
	m_resourceList = new wxListBox( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer18->Add( m_resourceList, 1, wxEXPAND|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	m_searchCtrl = new wxSearchCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifndef __WXMAC__
	m_searchCtrl->ShowSearchButton( true );
	#endif
	m_searchCtrl->ShowCancelButton( false );
	bSizer21->Add( m_searchCtrl, 1, wxBOTTOM|wxRIGHT|wxEXPAND, 5 );
	
	m_detailSlider = new wxSlider( m_panel3, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxSize( 60,-1 ), wxSL_HORIZONTAL );
	bSizer21->Add( m_detailSlider, 0, 0, 5 );
	
	bSizer18->Add( bSizer21, 0, wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer18 );
	m_panel3->Layout();
	bSizer18->Fit( m_panel3 );
	m_splitter2->SplitVertically( m_panel2, m_panel3, 121 );
	bSizer16->Add( m_splitter2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_resourceGroups->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourcesBrowser::onResourceGroupChanged ), NULL, this );
	m_resourceList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ResourcesBrowser::onResourceListSelection ), NULL, this );
}

ResourcesBrowser::~ResourcesBrowser()
{
	// Disconnect Events
	m_resourceGroups->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ResourcesBrowser::onResourceGroupChanged ), NULL, this );
	m_resourceList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ResourcesBrowser::onResourceListSelection ), NULL, this );
	
}
