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
	void showEntityProperties( const EntityPtr& node );

	// Creates a memory watch.
	void createMemoryWatch(const Class* type, void* object, MemoryWatch& watch);

	// Updates a memory watch.
	bool updateMemoryWatch(const Class* type, void* object);

	// Updates the memory watches.
	bool updateMemoryWatches();

protected:

	// Appends the type fields to the property grid.
	void appendObjectFields(const Class& type, void* object, bool newCategory = true);

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
	std::map<const Class*, MemoryWatch> memoryWatches;

	std::vector<wxPGProperty*> composed;
};

//-----------------------------------//

} } // end namespaces