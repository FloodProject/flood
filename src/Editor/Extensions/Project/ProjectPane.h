/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Editor.h"
#include "Project.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class ProjectPane : public wxTreeCtrl
{
public:

	ProjectPane(wxWindow* parent);

	void initIcons();

	// Adds a new project to the pane.
	void addProject(const ProjectPtr&);

	std::vector<ProjectPtr> projects;
};

//-----------------------------------//

NAMESPACE_EDITOR_END