/************************************************************************
*
* vapor3D Editor © (2008-2011)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Preferences.h"
#include "ImageUtils.h"
#include "EditorIcons.h"
#include "Core/PluginManager.h"
#include "EditorPlugin.h"
#include "Editor.h"
#include <wx/keybinder.h>

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

Preferences::Preferences( wxWindow* parent )
	: m_listPlugins(nullptr)
{
	if (!wxPropertySheetDialog::Create(parent, wxID_ANY, "Preferences"))
		return;

	createPages();
}

//-----------------------------------//

#define wxKEYBINDER_CUSTOM_STYLE (wxKEYBINDER_USE_TREECTRL | wxTAB_TRAVERSAL)

bool Preferences::createPages()
{
	CreateButtons(wxOK|wxCANCEL);

	wxImageList* imageList = new wxImageList(16, 16, true);
	imageList->Add(wxMEMORY_BITMAP(keyboard));
	imageList->Add(wxMEMORY_BITMAP(plugin));
	imageList->Add(wxMEMORY_BITMAP(package));
	imageList->Add(wxMEMORY_BITMAP(table));

	wxBookCtrlBase* nb = GetBookCtrl();	
	nb->AssignImageList(imageList);

	int n = 0;

	wxKeyConfigPanel* bindPanel = new wxKeyConfigPanel(nb, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxKEYBINDER_CUSTOM_STYLE);

	wxKeyProfileArray& profiles = GetEditor().keyProfiles;
	bindPanel->AddProfiles(profiles);
	//bindPanel->ImportKeyProfileCmd(*profiles.GetSelProfile());
	bindPanel->ImportMenuBarCmd( GetEditor().GetMenuBar() );

	nb->AddPage(bindPanel, "Key Bindings");
	nb->SetPageImage(n++, 0);

#if 0
	bindings = new Bindings(nb);
	nb->AddPage(bindings, "Key Bindings");
	nb->SetPageImage(n++, 0);
#endif

	plugins = new Plugins(nb);
    nb->AddPage(plugins, "Plugins");
	plugins->InitDialog();
	nb->SetPageImage(n++, 1);

	resources = new Resources(nb);
	nb->AddPage(resources, "Resources");
	resources->InitDialog();
	nb->SetPageImage(n++, 2);

	renderers = new Renderers(nb);
	nb->AddPage(renderers, "Renderers");
	renderers->InitDialog();
	nb->SetPageImage(n++, 3); 

    LayoutDialog();
    return true;
}

//-----------------------------------//

void Plugins::OnPluginInit(wxInitDialogEvent& event)
{
	wxListItem name;
	name.SetText("Name");
	m_listPlugins->InsertColumn(0, name);
	m_listPlugins->SetColumnWidth(0, 250);

	wxListItem version;
	version.SetText("Version");
	m_listPlugins->InsertColumn(1, version);

	wxListItem enabled;
	enabled.SetText("Enabled");
	m_listPlugins->InsertColumn(2, enabled);

	PluginManager* pm = GetEditor().getPluginManager();
	const std::vector<Plugin*>& plugins = pm->getPlugins();

	// Populate the list with the plugins.
	for( size_t i = 0; i < plugins.size(); i++ )
	{
		Plugin* plugin = plugins[i];
		PluginMetadata metadata = plugin->getMetadata();
		
		long id = m_listPlugins->InsertItem(m_listPlugins->GetItemCount(), wxEmptyString);
		m_listPlugins->SetItem(id, 0, metadata.name);
		m_listPlugins->SetItem(id, 1, metadata.version);
		m_listPlugins->SetItem(id, 2, plugin->isEnabled() ? "Yes" : "No");
		m_listPlugins->SetItemPtrData(id, (wxUIntPtr) plugin);

		if( !plugin->isEnabled() )
			m_listPlugins->SetItemTextColour(id, *wxLIGHT_GREY);
	}
}

//-----------------------------------//

void Plugins::OnPluginSelected( wxListEvent& event )
{
	long id = event.GetIndex();

	Plugin* plugin = (Plugin*) m_listPlugins->GetItemData(id);
	
	if( !plugin )
	{
		LogDebug("Problem occured selecting plugin");
		return;
	}

	const PluginMetadata& metadata = plugin->getMetadata();

	bool isEnabled = plugin->isEnabled();
	m_buttonPluginEnable->SetLabel(isEnabled ? "Disable" : "Enable");
	
	m_buttonPluginEnable->Enable(true);
	m_buttonPluginUninstall->Enable(true);

	m_textPluginDescription->SetValue( wxString(metadata.description) );
}

//-----------------------------------//

void Plugins::OnPluginEnable( wxCommandEvent& event )
{
	long id = m_listPlugins->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if(id < 0)
	{
		LogDebug("No plugin selection found");
		return;
	}

	Plugin* plugin = (Plugin*) m_listPlugins->GetItemData(id);

	if( !plugin )
	{
		LogDebug("Invalid plugin data");
		return;
	}

	bool isEnabled = plugin->isEnabled();

	PluginManager* pm = GetEditor().getPluginManager();

	if( !isEnabled )
	{
		pm->enablePlugin(plugin);
		m_listPlugins->SetItemTextColour(id, *wxBLACK);
		m_buttonPluginEnable->SetLabel("Disable");
	}
	else
	{
		pm->disablePlugin(plugin);
		m_listPlugins->SetItemTextColour(id, *wxLIGHT_GREY);
		m_buttonPluginEnable->SetLabel("Enable");
	}
}

//-----------------------------------//

void Plugins::OnPluginCheckUpdates( wxCommandEvent& event )
{
	int itemCount = m_listPlugins->GetItemCount();

	wxProgressDialog progress("Update check", "Checking for updates");
	progress.SetRange(itemCount);
	progress.Show();

	for(int i = 0; i < itemCount; i++)
	{
		wxSleep(1);
		wxYield();
		progress.Update(i+1);
	}
}

//-----------------------------------//

void Plugins::OnPluginInstall( wxCommandEvent& event )
{
	wxMessageBox("Not implemented yet");
}

//-----------------------------------//

void Resources::OnResourcesInit( wxInitDialogEvent& event )
{
	m_listResourcePaths->InsertColumn(0, "Path", wxLIST_FORMAT_LEFT, 400);
}

//-----------------------------------//

void Resources::OnResourcesPathAdd( wxCommandEvent& event )
{
	wxDirDialog dirPicker(nullptr, "Choose directory", wxEmptyString,
			wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	int res = dirPicker.ShowModal();
	if(res != wxID_OK) return;

	wxString path = dirPicker.GetPath();

	int itemCount = m_listResourcePaths->GetItemCount();
	m_listResourcePaths->InsertItem(itemCount, path);
}

//-----------------------------------//

NAMESPACE_EDITOR_END