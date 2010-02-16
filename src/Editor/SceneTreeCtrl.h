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

class SceneTreeCtrl : public wxTreeCtrl 
{
public:

	SceneTreeCtrl( vapor::Engine* engine, 
					wxWindow* parent, wxWindowID id = wxID_ANY,
					const wxPoint& pos	= wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS 
					| wxTR_NO_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT,
					const wxValidator& validator = wxDefaultValidator, 
					const wxString&	name = "SceneTreeCtrl" );
					
	virtual ~SceneTreeCtrl();

	// Refreshes the tree control with the scene contents.
	void updateScene( wxTreeItemId id, const NodePtr& node );

	// Gets the entity represented by the tree item.
	scene::NodePtr getEntity( wxTreeItemId id );

	// Sets the bounding box of a tree item to a given state.
	void setBoundingBox( const wxTreeItemId& id, bool state );


	fd::delegate<void (wxTreeItemId, wxTreeItemId)> onItemSelected;

protected:

	// Initializes the control and icons.
	void initControl();
	void initIcons();

	// wxWidgets events.
	void onItemChanged(wxTreeEvent& event);
	void onItemMenu(wxTreeEvent& event);

	// vapor Scene-monitoring callbacks.
	void onNodeAdded( const scene::GroupEvent& event );
	void onNodeRemoved( const scene::GroupEvent& event );

	// Callback when label editing ends.
	void onLabelEdit( wxTreeEvent& event );

	// Callback when control is activated.
	void onActivate( wxFocusEvent& event );

	// Dragging events.
	void onDragBegin( wxTreeEvent& event );
	void onDragEnd( wxTreeEvent& event );

	void onMouseRightUp( wxMouseEvent& event );
	void onNodeMenu( wxCommandEvent& event );

	// Scene associated this control.
	vapor::scene::SceneWeakPtr scene;

	// Holds the engine instance.
	vapor::Engine* engine;

	wxImageList* imageList;
	wxTreeItemId root;

	std::map< std::string, int > componentIcons;
	std::vector< NodePtr > selectedNodes;

	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;
	bool activated;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

enum 
{
	ID_SceneTree,
	ID_MenuSceneNodeDelete = wxID_DELETE,
	ID_MenuSceneNodeVisible,
	ID_MenuSceneNodeWireframe,
	ID_MenuSceneNodeAddMesh,
};

//-----------------------------------//

} } // end namespaces