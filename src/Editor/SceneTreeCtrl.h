/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

class EditorFrame;

//-----------------------------------//

/**
 * This control is responsible for mantaining and updating a TreeCtrl
 * with all the contents of the scene tree in the passed instance of
 * the engine. The events of this control (right-click context menus)
 * will be used to change some settings of the scene tree, and also
 * properties of the associated scene nodes.
 */

class SceneTreeCtrl : public wxPanel 
{
public:

	SceneTreeCtrl( EditorFrame*, Engine* engine, 
					wxWindow* parent, wxWindowID id = wxID_ANY );

	// Gets the entity represented by the tree item.
	NodePtr getEntity( wxTreeItemId id );

	// Event sent when a tree node selection occurs.
	fd::delegate<void (wxTreeItemId, wxTreeItemId)> onItemSelected;

protected:

	// Initializes the control and icons.
	void initControl();
	void initIcons();
	void initScene( wxTreeItemId id, const NodePtr& node );

	// wxWidgets events.
	void onItemChanged( wxTreeEvent& );
	void onItemMenu( wxTreeEvent& );
	void onLabelEditBegin( wxTreeEvent& );
	void onLabelEditEnd( wxTreeEvent& );
	void onActivate( wxFocusEvent& );
	void onDragBegin( wxTreeEvent& );
	void onDragEnd( wxTreeEvent& );
	void onMouseRightUp( wxContextMenuEvent& );
	void onNodeMenu( wxCommandEvent& );
	void onComponentAdd( wxCommandEvent& );

	wxBitmapButton* buttonNodeDelete;
	void onButtonNodeAdd(wxCommandEvent&);
	void onButtonNodeDelete(wxCommandEvent&);

	wxTreeCtrl* tree;
	wxTreeItemId root;
	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;
	wxTreeItemId addNode( const NodePtr& node );
	void addComponent( wxTreeItemId id, ComponentPtr  );

	// Scene-monitoring callbacks.
	void onNodeAdded( const GroupEvent& );
	void onNodeRemoved( const GroupEvent& );

	wxImageList* imageList;
	std::map<std::string, int> icons;

	// Scene associated this control.
	SceneWeakPtr weakScene;
	Engine* engine;
	EditorFrame* editor;

	bool activated;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces