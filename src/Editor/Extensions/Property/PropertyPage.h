/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PLUGIN_PROPERTY

#include <wx/foldbar/foldpanelbar.h>

#include "Core/ClassWatcher.h"

class wxPropertyGrid;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class PropertyData : public wxClientData
{
public:

	const Type* type;
	const Field* field;
	const void* object;
};

//-----------------------------------//

/**
 * This control is responsible for providing a page with a property editor.
 * It uses the reflection system provided by the engine to introspect the
 * types and allow easy editing of the fields.
 */

class PropertyGrid;
struct ClassWatch;
struct Class;

class PropertyPage : public wxFoldPanelBar
{
public:

	PropertyPage( wxWindow* parent );
	~PropertyPage();

	// Forces an update of the control (re-layout and refresh).
	void update();

	// Resets the properties page.
	void reset();

	// Resets the properties page if the object is current.
	void resetObject(const Object* object);

	// Gets/sets the current object.
	ACESSOR(Object, Object*, currentObject)

	// Creates a new property grid.
	PropertyGrid* createPropertyGrid(wxWindow* parent);

	// Appends the type fields to the property grid.
	void appendObjectFields( PropertyGrid*, Class*, void* object );

	// Populates properties on the grid.
	void showProperties( PropertyGrid*, Object* object, bool reset = true );

	// Gets the value of a field.
	wxAny getFieldValue(const Field* field, void* object);

	// Sets the value of a field.
	void setFieldValue(const Field* field, void* object, const wxAny& value);

	Delegate1<const FieldWatchVector&> onClassFieldChanged;

protected:

	// Gets the value of a property.
	wxAny getPropertyValue(wxPGProperty* property);

	// Gets the value of a primitive property.
	wxAny getPropertyPrimitiveValue(wxPGProperty* prop, PropertyData* data);

	// Sets the value of a property.
	void setPropertyValue(wxPGProperty* property, const wxAny& value);

	// Sets the value of a primitive property.
	void setPropertyPrimitiveValue(wxPGProperty* property, const wxAny& value);

	// Gets the value of a primitive field.
	wxAny getFieldPrimitiveValue(const Field* field, void* object);

	// Creates a property for a field.
	wxPGProperty* createProperty(Class& type, const Field& field, void* object);

	// Creates a new property for a primitive type.
	wxPGProperty* createPrimitiveProperty(const Field& field, void* object);

	// Creates a new property for a resource type.
	wxPGProperty* createResourceProperty(const Field& field, void* object);

	// Creates a new property for an enum type.
	wxPGProperty* createEnumProperty(const Field& field, void* object);

	// Creates a new property for a float type.
	wxFloatProperty* createFloatProperty(const char* name, float value);

	// Updates the property with the field value.
	void updatePrimitiveProperty(wxPGProperty* prop, const Field& field, void* object);

	// Callback when property value is about to change.
	void onPropertyChanging(wxPropertyGridEvent& event);

	// Callback when property value changes.
	void onPropertyChanged(wxPropertyGridEvent& event);

	// Callbacks
	void onIdle(wxIdleEvent& event);

	// Current property value.
	wxAny propertyValue;

	// Current object.
	Object* currentObject;

	// Current object watch.
	ClassWatch watch;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif