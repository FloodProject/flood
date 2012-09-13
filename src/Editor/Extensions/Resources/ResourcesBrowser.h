/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_RESOURCE_BROWSER

#include "GUI\PreferencesGenerated.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;

struct ResourceMetadata;
class ResourceDatabase;

typedef std::map< ResourceGroup, wxTreeItemId > ResourceGroupIdsMap;
typedef std::map< ResourceGroup, int > ResourceGroupIconsMap;

/**
 * A resources browser is a window where you can choose a resource from
 * the existing resouces of a given type.
 */

class ResourcesBrowser : public ResourcesFrame
{
public:

	ResourcesBrowser( wxWindow* parent );
	~ResourcesBrowser();

	// Sets the resource database.
	void setDatabase( ResourceDatabase* database );

	// Shows the resources from the given category.
	void showGroup(ResourceGroup group);

	// Selects the resources from the given group.
	void selectGroup(ResourceGroup group);

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

	// Sets up the resource gruops icons.
	void setupGroupIcons();
	
	// Sets up the UI.
	void setupUI();

	// Gets called when a resource is added to the dabase.
	void onResourceDatabaseAdded(const ResourceMetadata&);

	// Adds a resource group from a database.
	void addDatabaseGroup(ResourceDatabase* db, ResourceGroup group);

	// Adds a resource to the browser.
	void addResource(const ResourceMetadata& metadata);

	// Finds a resource group from a tree item id.
	bool findResourceGroupFromTreeId( wxTreeItemId id, ResourceGroup& group );

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
	ResourceGroup selectedGroup;
	ResourceDatabase* database;
	wxImageList* m_resourceImages;
	wxTreeItemId m_rootId;
	int listIndex;

	bool inSelectionMode;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif