/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"

namespace vapor { namespace editor {

//-----------------------------------//

class NodeOperation : public UndoOperation
{
public:

	void redo();
	void undo();

	NodePtr node;
	SceneWeakPtr weakScene;
};

//-----------------------------------//

class EditorFrame;
class NodeOperation;

#if defined(VAPOR_COMPILER_MSVC_2010) && defined(VAPOR_MEMORY_TR1_VENDOR)
typedef std::map<NodeWeakPtr, wxTreeItemId, std::owner_less<NodeWeakPtr>> NodeIdsMap;
#else
typedef std::map<NodeWeakPtr, wxTreeItemId> NodeIdsMap;
#endif

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

	// Gets the component associated with the tree item.
	ComponentPtr getComponentFromTreeId( wxTreeItemId id );

	// Gets the tree id from the node.
	wxTreeItemId getTreeIdFromNode(const NodePtr& node);

	// Did we send the last selection event.
	bool sentLastSelectionEvent;

protected:

	// Initializes the control.
	void initTree();
	void initButtons();

	// Initializes the icons list.
	void initIcons();

	// Adds a group node to the tree.
	void addGroup( wxTreeItemId id, const NodePtr& node, bool createGroup = true );

	// Adds a node to the tree.
	wxTreeItemId addNode( wxTreeItemId id, const NodePtr& node );

	// Adds a component to the tree item.
	void addComponent( wxTreeItemId id, ComponentPtr component );

	// Creates a new node operation.
	NodeOperation* createNodeOperation(const NodePtr& node);

	// Cleans the current scene.
	void cleanScene();

	// wxWidgets event callbacks.
	void onItemChanged( wxTreeEvent& );
	void onItemMenu( wxTreeEvent& );
	void onLabelEditBegin( wxTreeEvent& );
	void onLabelEditEnd( wxTreeEvent& );
	void onDragBegin( wxTreeEvent& );
	void onDragEnd( wxTreeEvent& );
	void onContextMenu( wxContextMenuEvent& );
	void onMenuSelected( wxCommandEvent& );
	void onComponentAdd( wxCommandEvent& );
	void onButtonNodeAdd(wxCommandEvent&);
	void onButtonNodeDelete(wxCommandEvent&);
	void onButtonNodeDeleteUpdate(wxUpdateUIEvent&);

	// Event helpers.
	MeshPtr askMeshResource();
	void populateNodeItemMenu(wxMenu& menu, const NodePtr& node);
	void populateComponentItemMenu(wxMenu& menu, const ComponentPtr& component);
	void onAnimationMenuSelected(wxCommandEvent&);
	void onAttachmentMenuSelected(wxCommandEvent&);
	wxMenu* createMenuAnimation(const MeshPtr& node);
	wxMenu* createMenuAttachment(const MeshPtr& node);
	int firstAnimationId;
	int firstAttachmentId;
	MeshPtr mesh;
	ModelPtr model;

	// Scene-monitoring callbacks.
	void onNodeAdded( const NodePtr& node );
	void onNodeRemoved( const NodePtr& node );

	// Scene tree.
	wxBoxSizer* sizer;
	wxTreeCtrl* treeCtrl;
	wxTreeItemId rootId;
	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;
	NodeIdsMap nodeIds;

	// Tree icons.
	wxImageList* imageList;
	std::map<const Type*, int> icons;
	std::map<const Type*, wxBitmap> bitmaps;

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

	// Node counter.
	int nodeCounter;
};

//-----------------------------------//

} } // end namespaces