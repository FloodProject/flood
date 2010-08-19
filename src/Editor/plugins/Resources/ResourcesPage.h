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

class EditorFrame;

//-----------------------------------//

/**
 * This control is responsible for mantaining and updating a TreeCtrl
 * with all the contents of the scene tree in the passed instance of
 * the engine. The events of this control (right-click context menus)
 * will be used to change some settings of the scene tree, and also
 * properties of the associated scene nodes.
 */

class ResourcesPage : public wxTreeCtrl 
{
public:

	ResourcesPage( EditorFrame* editor,
		wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	~ResourcesPage();

	// Synchronizes the tree with the resources.
	void updateTree();

protected:

	void initControl();
	void initIcons();

	void onItemMenu(wxTreeEvent& event);

	// Resource-monitoring callbacks.
	void onResourceAdded( const ResourceEvent& event );
	void onResourceRemoved( const ResourceEvent& event );
	void onResourceReloaded( const ResourceEvent& event );

	EditorFrame* editor;
	Engine* engine;
	ResourceManager* rm;

	wxImageList* imageList;
	wxTreeItemId root;

	std::map< ResourceGroup::Enum, wxTreeItemId > resourceGroupTreeIds;
	std::map< ResourceGroup::Enum, int > resourceGroupIcons;

private:

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces