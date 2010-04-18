/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * This control is responsible for mantaining and updating a TreeCtrl
 * with all the contents of the scene tree in the passed instance of
 * the engine. The events of this control (right-click context menus)
 * will be used to change some settings of the scene tree, and also
 * properties of the associated scene nodes.
 */

class ResourceTreeCtrl : public wxTreeCtrl 
{
public:

	ResourceTreeCtrl(vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id = wxID_ANY,
					const wxPoint& pos	= wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT,
					const wxValidator& validator = wxDefaultValidator, 
					const wxString&	name = "ResourceTreeCtrl");

	// Synchronizes the tree with the resources.
	void updateTree();

protected:

	void InitControl();
	void initIcons();

	void onItemMenu(wxTreeEvent& event);

	// Resource-monitoring callbacks.
	void onResourceAdded( const resources::ResourceEvent& event );
	void onResourceRemoved( const resources::ResourceEvent& event );

	Engine* engine;
	ResourceManagerPtr rm;

	wxImageList* imageList;

	wxTreeItemId root;

	std::map< resources::ResourceGroup::Enum, wxTreeItemId > resourceGroupTreeIds;
	std::map< resources::ResourceGroup::Enum, int > resourceGroupIcons;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces