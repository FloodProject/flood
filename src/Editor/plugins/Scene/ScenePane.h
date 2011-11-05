/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"

FWD_DECL_INTRUSIVE(Entity)
FWD_DECL_INTRUSIVE(Component)
FWD_DECL_INTRUSIVE(Scene)
FWD_DECL_INTRUSIVE(Model)
FWD_DECL_INTRUSIVE(Mesh)

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

enum 
{
	ID_MenuSceneEntityDelete = wxID_HIGHEST + 3654,
	ID_MenuSceneEntityDuplicate,
	ID_MenuSceneEntityVisible,
	ID_MenuSceneEntityWireframe,
	ID_MenuSceneEntityTerrain,
	ID_MenuSceneComponentRemove,
};

//-----------------------------------//

class SelectionOperation;

class EntityOperation : public UndoOperation
{
public:

	EntityOperation();

	enum OperationType { EntityAdded, EntityRemoved, ComponentAdded, ComponentRemoved };

	void undo() OVERRIDE;
	void redo() OVERRIDE;

	EntityOperation::OperationType type;

	EntityWeakPtr entity;
	ComponentWeakPtr component;
	SceneWeakPtr weakScene;
};

//-----------------------------------//

class EntityItemData : public wxTreeItemData
{
public:

	EntityItemData() : entity(nullptr) {}

	Entity* entity;
	ComponentWeakPtr component;
};

//-----------------------------------//

class EditorFrame;
class EntityOperation;

typedef std::map<Object*, wxTreeItemId> ObjectIdsMap;

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

	ScenePage( wxWindow* parent, wxWindowID id = wxID_ANY );
	virtual ~ScenePage();

	// Cleans the current scene.
	void cleanScene();

	// Sets the current scene.
	void setScene(const ScenePtr& scene);

	// Gets the tree control.
	GETTER(TreeCtrl, wxTreeCtrl*, treeCtrl)

	// Gets the tree item from the component.
	wxTreeItemId getTreeIdFromObject(Object*);

	// Gets the entity associated with the tree item.
	EntityPtr getEntityFromTreeId( wxTreeItemId id );

	// Gets the component associated with the tree item.
	ComponentPtr getComponentFromTreeId( wxTreeItemId id );

	// Did we send the last selection event.
	bool sentLastSelectionEvent;

	// Did we send the tree changing event.
	bool sentTreeChangingEvent;

	// Adds a component to the tree item.
	void addComponentToTree( wxTreeItemId id, ComponentPtr component );

protected:

	// Initializes the control.
	void initTree();
	void initButtons();
	void initIcons();

	// Tree management.
	void addGroup( wxTreeItemId id, const EntityPtr& node, bool createGroup = true );
	wxTreeItemId addEntity( wxTreeItemId id, const EntityPtr& node );

	// Creates a new entity operation.
	EntityOperation* createEntityOperation(const String& desc);
	EntityOperation* createEntityAddOperation(const EntityPtr&);

	// wxWidgets event callbacks.
	void onFocus(wxFocusEvent&);
	void onItemChanged( wxTreeEvent& );
	void onItemMenu( wxTreeEvent& );
	void onLabelEditBegin( wxTreeEvent& );
	void onLabelEditEnd( wxTreeEvent& );
	void onDragBegin( wxTreeEvent& );
	void onDragEnd( wxTreeEvent& );
	void onContextMenu( wxContextMenuEvent& );
	void onMenuSelected( wxCommandEvent& );
	void onComponentAdd( wxCommandEvent& );
	void onButtonEntityAdd(wxCommandEvent&);
	void onButtonEntityDelete(wxCommandEvent&);
	void onButtonEntityDeleteUpdate(wxUpdateUIEvent&);

	// Event helpers.
	void populateEntityItemMenu(wxMenu& menu, const EntityPtr& node);
	void populateComponentItemMenu(wxMenu& menu, const ComponentPtr& component);
	void onComponentMenuSelected(wxCommandEvent&);
	void onAnimationMenuSelected(wxCommandEvent&);
	void onAttachmentMenuSelected(wxCommandEvent&);
	wxMenu* createMenuAnimation(const MeshPtr& node);
	wxMenu* createMenuAttachment(const MeshPtr& node);
	int firstAnimationId;
	int firstAttachmentId;
	
	SceneWeakPtr weakScene;
	EntityWeakPtr weakEntity;
	ComponentWeakPtr weakComponent;

	ModelPtr model;
	MeshHandle meshHandle;
	Mesh* mesh;

	// Scene-monitoring callbacks.
	void onEntityAdded( const EntityPtr& );
	void onEntityRemoved( const EntityPtr& );
	void onComponentAdded( const ComponentPtr& );
	void onComponentRemoved( const ComponentPtr& );

	// Scene tree.
	wxBoxSizer* sizer;
	wxTreeCtrl* treeCtrl;
	wxTreeItemId rootId;
	wxTreeItemId menuItemId;
	wxTreeItemId dragItemId;
	ObjectIdsMap objectIds;

	// Tree icons.
	wxImageList* imageList;
	std::map<const Type*, int> icons;
	std::map<const Type*, wxBitmap> bitmaps;

	// Entity buttons.
	wxBitmapButton* buttonEntityAdd;
	wxBitmapButton* buttonEntityDelete;

	// Current menu.
	wxMenu* currentMenu;

	// Entity counter.
	int entityCounter;
};

wxImage GetIconFromComponent(Class* klass);

//-----------------------------------//

NAMESPACE_EDITOR_END