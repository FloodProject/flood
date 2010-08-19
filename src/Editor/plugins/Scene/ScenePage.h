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
typedef std::map<NodeWeakPtr, wxTreeItemId> NodeIdsMap;

//-----------------------------------//

class NodeItemData : public wxTreeItemData
{
public:

	NodeWeakPtr node;
	ComponentWeakPtr component;
};

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

	// Gets the tree control.
	GETTER(TreeCtrl, wxTreeCtrl*, treeCtrl)

	// Sets the current scene.
	void setScene(const ScenePtr& scene);

	// Gets the node associated with the tree item.
	NodePtr getNodeFromTreeId( wxTreeItemId id );

	// Gets the tree id from the node.
	wxTreeItemId getTreeIdFromNode(const NodePtr& node);

	// Did we send the last selection event.
	bool sentLastSelectionEvent;

protected:

	// Initializes the control.
	void initControl();

	// Initializes the icons list.
	void initIcons();

	// Adds a node and its children to the tree.
	void addNodeRecursively( wxTreeItemId id, const NodePtr& node );

	// Adds a new node to the tree.
	wxTreeItemId addNode( const NodePtr& node );

	// Adds a component to the tree item.
	void addComponent( wxTreeItemId id, ComponentPtr component );

	// wxWidgets event callbacks.
	void onItemChanged( wxTreeEvent& );
	void onItemMenu( wxTreeEvent& );
	void onLabelEditBegin( wxTreeEvent& );
	void onLabelEditEnd( wxTreeEvent& );
	void onDragBegin( wxTreeEvent& );
	void onDragEnd( wxTreeEvent& );
	void onMouseRightUp( wxContextMenuEvent& );
	void onNodeMenu( wxCommandEvent& );
	void onComponentAdd( wxCommandEvent& );
	void onButtonNodeAdd(wxCommandEvent&);
	void onButtonNodeDelete(wxCommandEvent&);
	void onButtonNodeDeleteUpdate(wxUpdateUIEvent&);

	// Scene-monitoring callbacks.
	void onNodeAdded( const GroupEvent& );
	void onNodeRemoved( const GroupEvent& );

	// Scene tree.
	wxTreeCtrl* treeCtrl;
	wxTreeItemId rootId;
	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;
	NodeIdsMap nodeIds;

	// Tree icons.
	wxImageList* imageList;
	std::map<const Type*, int> icons;

	// Node buttons.
	wxBitmapButton* buttonNodeAdd;
	wxBitmapButton* buttonNodeDelete;

	// Current menu.
	wxMenu* currentMenu;

	// Editor instance.
	EditorFrame* editor;

	// Engine instance.
	Engine* engine;

	// Scene instance.
	SceneWeakPtr weakScene;
	
private:

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces