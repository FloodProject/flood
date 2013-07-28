/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/ClassWatcher.h"  
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Math/Hash.h"
#include "Core/Containers/Hash.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

ClassWatch::ClassWatch()
	:fields(*AllocatorGetHeap())
{}

//-----------------------------------//

ClassWatch* ClassWatchCreate(Allocator* alloc)
{
	ClassWatch* watch = Allocate(alloc, ClassWatch);
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
	auto fw = watch->fields.get((uint64)field, FieldWatch());

	byte* min = (byte*) ClassGetFieldAddress(fw.object, field);
	byte* max = min + field->size;

	size_t size = max - min;
	uint32 hash = HashMurmur2(0xF00D, min, size);

	bool changed = false;

	if(hash != fw.hash)
	{
		fw.hash = hash;
		changed = true;

		watch->fields.set((uint64)field, fw);
	}

	return changed;
}

//-----------------------------------//

void ClassWatchAddField(ClassWatch* watch, const FieldWatch& fw )
{
	watch->fields.set((uint64)fw.field, fw);
	ClassWatchUpdateField(watch, fw.field);
}

//-----------------------------------//

void ClassWatchAddFields(ClassWatch* watch, Object* object)
{
	if( !watch || !object ) return;

	Class* klass = ClassGetType(object);
	const Array<Field*>& fields = klass->fields;

	for(size_t i = 0; i < fields.size(); ++i)
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
	for(auto fw : watch->fields)
	{
		bool updated = ClassWatchUpdateField(watch, fw.value.field);
		if( updated ) changed.push_back(&fw.value);
	}
}

//-----------------------------------//

NAMESPACE_CORE_END