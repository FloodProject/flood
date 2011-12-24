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

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(PropertyOperation)

class PropertyOperation : public UndoOperation
{
	REFLECT_DECLARE_OBJECT(PropertyOperation)

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

NAMESPACE_EDITOR_END

#endif