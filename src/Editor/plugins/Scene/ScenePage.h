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

class ScenePage : public wxPanel 
{
public:

	ScenePage( EditorFrame*, wxWindow* parent, wxWindowID id = wxID_ANY );
	virtual ~ScenePage();

	// Gets the entity represented by the tree item.
	NodePtr getEntity( wxTreeItemId id );

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
	void onDragBegin( wxTreeEvent& );
	void onDragEnd( wxTreeEvent& );
	void onMouseRightUp( wxContextMenuEvent& );
	void onNodeMenu( wxCommandEvent& );
	void onComponentAdd( wxCommandEvent& );

	wxTreeItemId addNode( const NodePtr& node );
	void addComponent( wxTreeItemId id, ComponentPtr  );

	void onButtonNodeAdd(wxCommandEvent&);
	void onButtonNodeDelete(wxCommandEvent&);
	void onButtonNodeDeleteUpdate(wxUpdateUIEvent&);

	// Scene-monitoring callbacks.
	void onNodeAdded( const GroupEvent& );
	void onNodeRemoved( const GroupEvent& );

	// Scene tree.
	wxTreeCtrl* tree;
	wxTreeItemId root;
	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;

	// Tree icons.
	wxImageList* imageList;
	std::map<std::string, int> icons;

	// Node buttons.
	wxBitmapButton* buttonNodeAdd;
	wxBitmapButton* buttonNodeDelete;

	// Scene associated this control.
	Engine* engine;
	EditorFrame* editor;
	SceneWeakPtr weakScene;
	
	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces