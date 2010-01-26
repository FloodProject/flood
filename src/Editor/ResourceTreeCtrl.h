/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
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
					
	virtual ~ResourceTreeCtrl();

	// Synchronizes the tree with the resources.
	void updateTree();

	vapor::Engine* getEngine() { return engine; }
	
	void setEngine(vapor::Engine* engine) { this->engine = engine; }
	
protected:

	void InitControl();
	void initIcons();

	void onItemMenu(wxTreeEvent& event);

	// vapor Resource-monitoring callbacks.
	void onResourceAdded( const resources::ResourceEvent& event );
	void onResourceRemoved( const resources::ResourceEvent& event );

	// Resource manager associated this control.
	resources::ResourceManager* rm;

	// Holds an instance of the engine.
	vapor::Engine* engine;

	wxImageList* imageList;

	wxTreeItemId root;

	//int blankImage;

	std::map< resources::ResourceGroup::Enum, wxTreeItemId > resourceGroupTreeIds;

	std::map< resources::ResourceGroup::Enum, int > resourceGroupIcons;

	DECLARE_EVENT_TABLE()
};

enum 
{
	ID_ResourceTree,
	//ID_MenuSceneNodeDelete = wxID_DELETE
};

//-----------------------------------//

} } // end namespaces