/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "EditorPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ResourcesPage;
class ResourcesBrowser;
class ResourceDatabase;
class ResourceThumbnailer;

//-----------------------------------//

REFLECT_DECLARE_CLASS(ResourcesPlugin)

class ResourcesPlugin : public EditorPlugin
{
	REFLECT_DECLARE_OBJECT(ResourcesPlugin)

public:

	ResourcesPlugin();
	~ResourcesPlugin();

	// Gets metadata about this plugin.
	PluginMetadata getMetadata() OVERRIDE;

	// Plugin enable callback.
	void onPluginEnable() OVERRIDE;

	// Plugin disable callback.
	void onPluginDisable() OVERRIDE;

	// Loads the thumbnails cache.
	bool loadCache();

	// Saves the thumbnails cache.
	bool saveCache();

	// Handles button click.
	void onBrowserButtonClick(wxCommandEvent& event);

	ResourceDatabase* resourceDatabase;
	ResourceThumbnailer* resourceThumbnailer;

	ResourcesPage* resourcesPage;
	ResourcesBrowser* resourcesBrowser;
	wxAuiToolBarItem* resourcesBrowserButton;
	int iconResources;
};

//-----------------------------------//

NAMESPACE_EDITOR_END