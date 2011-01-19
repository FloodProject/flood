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

class PropertyData : public wxClientData
{
public:

	const Type* type;
	const Field* field;
	const void* object;
};

//-----------------------------------//

struct MemoryWatch
{
	wxPGProperty* property;
	void* object;

	byte* rangeBegin;
	byte* rangeEnd;
	uint hash;
};

//-----------------------------------//

/**
 * This control is responsible for providing a page with a property editor.
 * It uses the reflection system provided by the engine to introspect the
 * types and allow easy editing of the fields.
 */

class PropertyPage : public wxPropertyGrid
{
public:

	PropertyPage( wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	// Populates properties on the grid.
	void showEntityProperties( const EntityPtr& entity );

	// Gets the value of a field.
	wxAny getFieldValue(const Field* field, void* object);

	// Sets the value of a field.
	void setFieldValue(const Field* field, void* object, const wxAny& value);

	// Updates the memory watches.
	bool updateMemoryWatches();

	// Creates a memory watch.
	void createMemoryWatch(const Field* field, void* object, MemoryWatch& watch);

	// Updates a memory watch.
	bool updateMemoryWatch(const Field* field, void* object);

	// Resets the properties page.
	void reset();

protected:

	// Appends the type fields to the property grid.
	void appendObjectFields(const Class& type, void* object, bool newCategory = true);

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
	wxPGProperty* createProperty(const Class& type, const Field& field, void* object);

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

	// Selected node.
	EntityWeakPtr selectedEntity;

	// Memory watches.
	typedef std::map<const Field*, MemoryWatch> MemoryWatchesMap;
	MemoryWatchesMap memoryWatches;

	std::vector<wxPGProperty*> composed;
};

//-----------------------------------//

} } // end namespaces