/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ProjectPane.h"

#include "Editor.h"
#include "EditorIcons.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

ProjectPane::ProjectPane(wxWindow* parent)
	: wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxTR_DEFAULT_STYLE | wxTR_TWIST_BUTTONS | wxBORDER_NONE | wxTR_NO_LINES)
{
	// Initialize the icons for the tree control.
	initIcons();
}

//-----------------------------------//

enum TreeIcons
{
	Package = 0,
	FolderBrick,
	PageEmpty
};

void ProjectPane::addProject(const ProjectPtr& project)
{
	if( !project ) return;
	projects.push_back(project);

	String full = StringFormat("Project (%s)", project->name.c_str());
	this->AddRoot(full, FolderBrick);
}

//-----------------------------------//

void ProjectPane::initIcons()
{
	// Create a new list with all the icons.
	wxImageList* imageList = new wxImageList(16, 16, false, 16);
	
	imageList->Add(wxMEMORY_BITMAP(package));
	imageList->Add(wxMEMORY_BITMAP(folder_brick));
	imageList->Add(wxMEMORY_BITMAP(page_empty));
	
	this->AssignImageList(imageList);
}

//-----------------------------------//

NAMESPACE_EDITOR_END