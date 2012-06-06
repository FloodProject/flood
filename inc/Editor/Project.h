/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/Object.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

/** Using the project context map you can store project-specific data. */

struct ProjectContext : public Object
{
};

typedef std::map<void*, RefPtr<ProjectContext>> ProjectContextMap;

//-----------------------------------//

/**
 * Project items are the things that are inside the project.
 */

REFLECT_DECLARE_CLASS(ProjectItem)

struct ProjectItem : public Object
{
	REFLECT_DECLARE_OBJECT(ProjectItem)

	ProjectItem();
	virtual ~ProjectItem();

	// Adds an item to the project.
	void addItem( const ProjectItem& item );

	// Removes an item from the project.
	void removeItem( const ProjectItem& item );

	// Adds a new folder to the project root.
	ProjectItem* addFolder(const String& name);

	// Event when an item is added to the project.
	Event1<const ProjectItem&> onItemAdded;

	// Event when an item is removed from the project.
	Event1<const ProjectItem&> onItemRemoved;

	// Name of the item.
	String name;

	// Parent of the item.
	ProjectItem* parent;
};

typedef std::vector<ProjectItem> ProjectItemsVector;

//-----------------------------------//

class ResourceDatabase;

/**
 * Projects act as a container to the imported resources and keeps settings.
 */

REFLECT_DECLARE_CLASS(Project)

class Project : public ProjectItem
{
	REFLECT_DECLARE_OBJECT(Project)

public:

	Project();
	virtual ~Project();

	// Keeps the context data of the project.
	ProjectContextMap projectContext;

	// Keeps a database with resources.
	ResourceDatabase* database;

protected:
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Project)

//-----------------------------------//

NAMESPACE_EDITOR_END