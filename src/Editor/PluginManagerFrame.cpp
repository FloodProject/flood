/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PluginManagerFrame.h"
#include "PluginManager.h"
#include "Plugin.h"

namespace vapor { namespace editor {

//-----------------------------------//

PluginManagerFrame::PluginManagerFrame( wxWindow* parent, PluginManager* plugins )
	: wxCheckListBox(parent, wxID_ANY)
	, plugins(plugins)
{
	InitControl();
}

//-----------------------------------//

void PluginManagerFrame::InitControl()
{
	Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, &PluginManagerFrame::OnCheckEvent, this);

	const std::vector<Plugin*>& plugs = plugins->getPlugins();

	// Populate the list with the plugins.
	for( size_t i = 0; i < plugs.size(); i++ )
	{
		Plugin* plugin = plugs[i];
		PluginMetadata metadata = plugin->getMetadata();
		
		int n = Append( metadata.name, (void*) plugin );
		Check( n, plugin->isPluginEnabled() );
	}
}

//-----------------------------------//

void PluginManagerFrame::OnCheckEvent(wxCommandEvent& event)
{
	assert( GetId() == event.GetId() );

	int n = event.GetSelection();

	// Get the plugin stored in the custom data.
	Plugin* plugin = (Plugin*) GetClientData(n);
	assert( plugin != nullptr );

	bool doEnable = !plugin->isPluginEnabled();

	if(doEnable)
		plugins->enablePlugin( plugin );
	else
		plugins->disablePlugin( plugin );
}

//-----------------------------------//

} } // end namespaces