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

class PropertyOperation : public UndoOperation
{
public:
	
	Class* type;
	Field* field;
	void* object;
	
	wxAny prevValue;
	wxAny newValue;

	void undo();
	void redo();
	void setFieldValue(const wxAny& value);
};

//-----------------------------------//

} } // end namespaces