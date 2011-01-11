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

PluginManagerFrame::PluginManagerFrame( wxWindow* parent, PluginManager* plugins )
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
	, plugins(plugins)
{
	InitControl();
}

//-----------------------------------//

void PluginManagerFrame::InitControl()
{
	checkListBox = new wxCheckListBox(this, wxID_ANY);
	checkListBox->Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, &PluginManagerFrame::OnCheckEvent, this);

	const std::vector<Plugin*>& plugs = plugins->getPlugins();

	// Populate the list with the plugins.
	for( uint i = 0; i < plugs.size(); i++ )
	{
		Plugin* plugin = plugs[i];
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
		plugins->enablePlugin( plugin );
	else
		plugins->disablePlugin( plugin );
}

//-----------------------------------//

} } // end namespaces