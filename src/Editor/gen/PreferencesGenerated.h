///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 22 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PreferencesGenerated__
#define __PreferencesGenerated__

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

///////////////////////////////////////////////////////////////////////////

namespace vapor
{
	namespace gui
	{
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
		
	} // namespace gui
} // namespace vapor

#endif //__PreferencesGenerated__
