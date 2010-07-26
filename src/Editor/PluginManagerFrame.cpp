/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PluginManagerFrame.h"
#include "PluginManager.h"
#include "Plugin.h"

namespace vapor { namespace editor {

//-----------------------------------//
// Event handlers
//-----------------------------------//

BEGIN_EVENT_TABLE(PluginManagerFrame, wxFrame)
	EVT_CLOSE( PluginManagerFrame::OnClose )
	EVT_CHECKLISTBOX( wxID_ANY, PluginManagerFrame::OnCheckEvent )
END_EVENT_TABLE()

//-----------------------------------//

PluginManagerFrame::PluginManagerFrame( wxWindow* parent, PluginManager* pluginManager )
	: wxFrame(parent, wxID_ANY, "Plugin Manager", wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_FRAME_STYLE | wxFRAME_TOOL_WINDOW | wxFRAME_FLOAT_ON_PARENT | wxBORDER_NONE )
	, pluginManager(pluginManager)
{
	InitControl();
}

//-----------------------------------//

void PluginManagerFrame::InitControl()
{
	checkListBox = new wxCheckListBox(this, wxID_ANY);

	// Populate the list with the plugins.
	foreach( Plugin* plugin, pluginManager->getPlugins() )
	{
		PluginMetadata metadata = plugin->getMetadata();
		
		int n = checkListBox->Append( metadata.name, (void*) plugin );
		checkListBox->Check( n, plugin->isPluginEnabled() );
	}
}

//-----------------------------------//

void PluginManagerFrame::OnCheckEvent(wxCommandEvent& event)
{
	assert( checkListBox->GetId() == event.GetId() );

	int n = event.GetSelection();

	// Get the plugin stored in the custom data.
	Plugin* plugin = (Plugin*) checkListBox->GetClientData(n);
	assert( plugin != nullptr );

	bool doEnable = !plugin->isPluginEnabled();

	if(doEnable)
		pluginManager->enablePlugin( plugin );
	else
		pluginManager->disablePlugin( plugin );
}

//-----------------------------------//

void PluginManagerFrame::OnClose(wxCloseEvent& event)
{
    if( event.CanVeto() )
    {
		// Hide the window instead of closing it.
		Hide();

        event.Veto();
        return;   
    }

    Destroy();  // you may also do: event.Skip();
}

//-----------------------------------//

} } // end namespaces