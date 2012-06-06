/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

#include "Editor.h"
#include "Project.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(ProjectItem, Object)
REFLECT_CLASS_END()

ProjectItem::ProjectItem()
	: parent(nullptr)
{
}

//-----------------------------------//

ProjectItem::~ProjectItem()
{
}

//-----------------------------------//

void ProjectItem::addItem( const ProjectItem& item )
{
	onItemAdded(item);
}

//-----------------------------------//

void ProjectItem::removeItem( const ProjectItem& item )
{
	//auto it = std::find(items.begin(), items.end(), item);
	//
	//if( it != items.end() )
	//	items.erase(it);

	onItemRemoved(item);
}

//-----------------------------------//

REFLECT_CHILD_CLASS(Project, ProjectItem)
REFLECT_CLASS_END()

Project::Project()
{
}

//-----------------------------------//

Project::~Project()
{
}

//-----------------------------------//

NAMESPACE_EDITOR_END