/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"
#include "PropertyPage.h"

#ifdef ENABLE_PLUGIN_PROPERTY

namespace vapor { namespace editor {

//-----------------------------------//

class PropertyOperation : public UndoOperation
{
public:
	
	Class* type;
	Field* field;
	void* object;

	wxAny oldValue;
	wxAny newValue;

	PropertyPage* grid;

	void undo();
	void redo();
	void setFieldValue(const wxAny& value);
};

//-----------------------------------//

} } // end namespaces

#endif