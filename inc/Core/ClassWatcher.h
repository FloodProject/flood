/************************************************************************
*
* vapor3D Core © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Object;
struct Class;
struct Field;

/**
 * Class watchers can be used to check for changes in reflected classes.
 * It works by hashing the content of the class fields and notifying the
 * code whenever the value changes.
 */

struct FieldWatch
{
	void* userdata;
	const void* object;
	const Field* field;
	uint32 hash;
};

typedef std::map<const Field*, FieldWatch> FieldWatchMap;
typedef std::vector<FieldWatch*> FieldWatchVector;

struct ClassWatch
{
	FieldWatchMap fields;
};

// Creates a new class watch.
API_CORE ClassWatch* ClassWatchCreate(Allocator*);

// Resets an existing class watch.
API_CORE void ClassWatchReset(ClassWatch*);

// Adds a field to the class watch.
API_CORE void ClassWatchAddField(ClassWatch*, const FieldWatch& fw );

// Adds the class fields to the class watch.
API_CORE void ClassWatchAddFields(ClassWatch*, Object* object);

// Updates a class field in the class watch.
API_CORE bool ClassWatchUpdateField(ClassWatch*, const Field* field);

// Updates the watches and gets the changed fields.
API_CORE void ClassWatchUpdate(ClassWatch*, FieldWatchVector& changed);

//-----------------------------------//

NAMESPACE_CORE_END