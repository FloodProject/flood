/************************************************************************
*
* vapor3D Core © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/ClassWatcher.h"
#include "Core/Memory.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Math/Hash.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

ClassWatch* ClassWatchCreate(Allocator* alloc)
{
	ClassWatch* watch = Allocate(ClassWatch, alloc);
	return watch;
}

//-----------------------------------//

void ClassWatchReset(ClassWatch* watch)
{
	if( !watch ) return;
	watch->fields.clear();
}

//-----------------------------------//

bool ClassWatchUpdateField(ClassWatch* watch, const Field* field)
{
	FieldWatch& fw = watch->fields[field];

	byte* min = (byte*) ClassGetFieldAddress(fw.object, field);
	byte* max = min + field->size;

	size_t size = max - min;
	uint32 hash = HashMurmur2(0xF00D, min, size);

	bool changed = false;

	if(hash != fw.hash)
	{
		fw.hash = hash;
		changed = true;
	}

	return changed;
}

//-----------------------------------//

void ClassWatchAddField(ClassWatch* watch, const FieldWatch& fw )
{
	watch->fields[fw.field] = fw;
	ClassWatchUpdateField(watch, fw.field);
}

//-----------------------------------//

void ClassWatchAddFields(ClassWatch* watch, Object* object)
{
	if( !watch || !object ) return;

	Class* klass = ClassGetType(object);
	const std::vector<Field*>& fields = klass->fields;

	for(size_t i = 0; i < fields.size(); i++)
	{
		const Field* field = fields[i];

		FieldWatch fw;
		fw.object = object;
		fw.field = field;

		ClassWatchAddField(watch, fw);
	}
}

//-----------------------------------//

void ClassWatchUpdate(ClassWatch* watch, FieldWatchVector& changed)
{
	FieldWatchMap& watches = watch->fields;
	FieldWatchMap::iterator it = watches.begin();

	for(; it != watches.end(); ++it)
	{
		FieldWatch& fw = it->second;
		bool updated = ClassWatchUpdateField(watch, fw.field);
		if( updated ) changed.push_back(&fw);
	}
}

//-----------------------------------//

NAMESPACE_CORE_END