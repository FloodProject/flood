/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "gen\PreferencesGenerated.h"

namespace vapor { namespace editor {

//-----------------------------------//

class EditorFrame;

typedef std::map< ResourceGroup::Enum, wxTreeItemId > ResourceGroupIdsMap;
typedef std::map< ResourceGroup::Enum, int > ResourceGroupIconsMap;

/**
 * A resources browser is a window where you can choose a resource from
 * the existing resouces of a given type.
 */

class ResourcesBrowser : public gui::ResourcesBrowser
{
public:

	ResourcesBrowser( wxWindow* parent );

	// Shows the resources from the given category.
	void showCategory(ResourceGroup::Enum group);

	// Sets up the images.
	void setupImages();

protected:

	// Sets up the resource groups.
	void setupGroups();
	void setupGroupIcons();

	// Sets up the UI.
	void setupUI();

	// Finds a resource group from a tree item id.
	bool findResourceGroupFromTreeId( wxTreeItemId id, ResourceGroup::Enum& group );

	// Event handlers.
	void OnClose(wxCloseEvent& event);
	void OnListBeginDrag(wxListEvent& event);
	void onConnectClicked(wxCommandEvent& event);
	void onResourceGroupChanged( wxTreeEvent& event ) OVERRIDE;
	void onResourceListSelection( wxCommandEvent& event ) OVERRIDE;

	wxImageList* m_resourceGroupsImages;
	ResourceGroupIdsMap m_resourceGroupIds;
	ResourceGroupIconsMap m_resourceGroupImagesMap;

	wxTreeItemId m_rootId;
	int listIndex;

	//ResourceThumbnailer thumbnailer;
};

//-----------------------------------//

} } // end namespaces