/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "gen\PreferencesGenerated.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;
struct ResourceMetadata;

typedef std::map< ResourceGroup::Enum, wxTreeItemId > ResourceGroupIdsMap;
typedef std::map< ResourceGroup::Enum, int > ResourceGroupIconsMap;

/**
 * A resources browser is a window where you can choose a resource from
 * the existing resouces of a given type.
 */

class ResourcesBrowser : public ResourcesFrame
{
public:

	ResourcesBrowser( wxWindow* parent );

	// Shows the resources from the given category.
	void showGroup(ResourceGroup::Enum group);

	// Selects the resources from the given group.
	void selectGroup(ResourceGroup::Enum group);

	// Sets the focus to the search control.
	void setFocusToSearch();

	// Sets up the browser for resource selection.
	void enableSelection();
	void disableSelection();

	// Gets the resource list.
	GETTER(ListCtrl, wxListCtrl*, m_resourceList)

protected:

	// Sets up the resource groups.
	void setupGroups();
	void setupGroupIcons();
	
	// Sets up the UI.
	void setupUI();

	// Finds a resource group from a tree item id.
	bool findResourceGroupFromTreeId( wxTreeItemId id, ResourceGroup::Enum& group );

	// Gets the image index for a given resource.
	int getImageIndex( const ResourceMetadata& metadata );

	// Event handlers.
	void OnClose(wxCloseEvent& event) OVERRIDE;
	void OnListBeginDrag(wxListEvent& event) OVERRIDE;
	void onResourceGroupChanged( wxTreeEvent& event ) OVERRIDE;
	void onResourceListSelection( wxListEvent& event ) OVERRIDE;
	void onResourceSliderScroll( wxScrollEvent& event ) OVERRIDE;
	void onResourceListActivated( wxListEvent& event ) OVERRIDE;

	wxImageList* m_resourceGroupsImages;
	ResourceGroupIdsMap m_resourceGroupIds;
	ResourceGroupIconsMap m_resourceGroupImagesMap;

	wxImageList* m_resourceImages;
	wxTreeItemId m_rootId;
	int listIndex;

	bool inSelectionMode;
};

//-----------------------------------//

NAMESPACE_EDITOR_END