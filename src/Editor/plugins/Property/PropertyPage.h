/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PLUGIN_PROPERTY

#include "Core/ClassWatcher.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class PropertyData : public wxClientData
{
public:

	const Type* type;
	const Field* field;
	const void* object;
};

struct ClassWatch;
struct Class;

//-----------------------------------//

/**
 * This control is responsible for providing a page with a property editor.
 * It uses the reflection system provided by the engine to introspect the
 * types and allow easy editing of the fields.
 */

class PropertyPage : public wxPropertyGrid
{
public:

	PropertyPage( wxWindow* parent );

	// Sets the current object.
	SETTER(Object, Object*, currentObject)

	// Populates properties on the grid.
	void showProperties( Object* object, bool reset = true );

	// Appends a separator header to the property grid.
	void appendHeader( const String& name );

	// Appends the type fields to the property grid.
	void appendObjectFields(Class*, void* object);

	// Gets the value of a field.
	wxAny getFieldValue(const Field* field, void* object);

	// Sets the value of a field.
	void setFieldValue(const Field* field, void* object, const wxAny& value);

	// Resets the properties page.
	void reset();

	// Resets the properties page if the object is current.
	void resetObject(const Object* object);

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

	// Callback when idle.
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