///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PREFERENCESGENERATED_H__
#define __PREFERENCESGENERATED_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/filepicker.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/srchctrl.h>
#include <wx/slider.h>
#include <wx/gauge.h>
#include <wx/splitter.h>
#include <wx/dialog.h>
#include <wx/valtext.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class Bindings
///////////////////////////////////////////////////////////////////////////////
class Bindings : public wxPanel 
{
	private:
	
	protected:
		wxPanel* m_panelBindings;
		wxStaticText* m_staticText2;
		wxTreeCtrl* m_treeCtrl1;
		wxStaticText* m_staticText3;
		wxListBox* m_listBox1;
		wxButton* m_button1;
		wxButton* m_button2;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrl2;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrl3;
		wxButton* m_button3;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl1;
	
	public:
		
		Bindings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL ); 
		~Bindings();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Plugins
///////////////////////////////////////////////////////////////////////////////
class Plugins : public wxPanel 
{
	private:
	
	protected:
		wxListCtrl* m_listPlugins;
		wxButton* m_buttonPluginEnable;
		wxButton* m_buttonPluginUninstall;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonPluginCheckUpdates;
		wxButton* m_buttonPluginInstall;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_textPluginDescription;
		
		// Virtual event handlers, overide them in your derived class
		void OnPluginInit( wxInitDialogEvent& event );
		void OnPluginSelected( wxListEvent& event );
		void OnPluginEnable( wxCommandEvent& event );
		void OnPluginCheckUpdates( wxCommandEvent& event );
		void OnPluginInstall( wxCommandEvent& event );
		
	
	public:
		
		Plugins( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL ); 
		~Plugins();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Resources
///////////////////////////////////////////////////////////////////////////////
class Resources : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText21;
		wxListCtrl* m_listResourcePaths;
		wxButton* m_button10;
		wxButton* m_button11;
		wxCheckBox* m_checkBox1;
		wxStaticText* m_staticText8;
		wxDirPickerCtrl* m_dirPicker1;
		wxButton* m_button14;
		
		// Virtual event handlers, overide them in your derived class
		void OnResourcesInit( wxInitDialogEvent& event );
		void OnResourcesPathAdd( wxCommandEvent& event );
		
	
	public:
		
		Resources( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL ); 
		~Resources();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Renderers
///////////////////////////////////////////////////////////////////////////////
class Renderers : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText8;
		wxChoice* m_choice1;
		wxStaticText* m_staticText10;
		wxListCtrl* m_listCtrl2;
		wxStaticText* m_staticText9;
		wxListBox* m_listBox3;
	
	public:
		
		Renderers( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL ); 
		~Renderers();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ResourcesFrame
///////////////////////////////////////////////////////////////////////////////
class ResourcesFrame : public wxDialog 
{
	private:
	
	protected:
		wxSplitterWindow* m_splitter2;
		wxPanel* m_panel2;
		wxTreeCtrl* m_resourceGroups;
		wxPanel* m_panel3;
		wxSearchCtrl* m_searchCtrl;
		wxStaticText* m_staticText17;
		wxSlider* m_detailSlider;
		wxListCtrl* m_resourceList;
		wxGauge* m_gauge1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void onResourceGroupChanged( wxTreeEvent& event ) { event.Skip(); }
		virtual void onResourceSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnListBeginDrag( wxListEvent& event ) { event.Skip(); }
		virtual void onResourceListActivated( wxListEvent& event ) { event.Skip(); }
		virtual void onResourceListSelection( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		
		ResourcesFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 580,370 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ResourcesFrame();
		
		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( 121 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( ResourcesFrame::m_splitter2OnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ServerFrame
///////////////////////////////////////////////////////////////////////////////
class ServerFrame : public wxFrame 
{
	private:
	
	protected:
		wxNotebook* m_notebookServer;
		wxPanel* m_panelStatus;
		wxStaticText* m_staticText11;
		wxTextCtrl* m_textHost;
		wxStaticText* m_staticText12;
		wxTextCtrl* m_textPort;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_textName;
		wxStaticText* m_staticText15;
		wxChoice* m_choiceAuth;
		wxStaticText* m_staticText13;
		wxStaticText* m_textStatus;
		wxButton* m_buttonConnect;
		wxPanel* m_panelChat;
		wxTextCtrl* m_textChat;
		wxListBox* m_listBox3;
		wxTextCtrl* m_textMessage;
		wxButton* m_button10;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onConnectButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onChatSendButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxString port; 
		
		ServerFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Server"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxFRAME_TOOL_WINDOW|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
		
		~ServerFrame();
	
};

#endif //__PREFERENCESGENERATED_H__
