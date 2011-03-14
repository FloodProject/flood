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

class ResourceDropTarget : public wxDropTarget
{
public:

	ResourceDropTarget(EditorFrame* frame);
	wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);

protected:

	wxTextDataObject* data;
	EditorFrame* frame;
};

//-----------------------------------//

ResourceDropTarget::ResourceDropTarget(EditorFrame* frame)
	: frame(frame)
{
	data = new wxTextDataObject();
	SetDataObject(data);
}

//-----------------------------------//

wxDragResult ResourceDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	Vector2 coords(x, y);
	frame->setDropCoords(coords);

	return def;
}

//-----------------------------------//

} } // end namespaces