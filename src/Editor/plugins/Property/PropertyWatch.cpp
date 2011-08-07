/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PropertyPage.h"
#include "Math/Hash.h"

#ifdef ENABLE_PLUGIN_PROPERTY

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

void PropertyPage::createMemoryWatch(const Field* field, void* object, MemoryWatch& watch)
{
	byte* min = (byte*) object + field->offset;
	byte* max = min + field->size;

	std::vector<byte> memory;
	memory.resize(max - min);
	
	std::copy( min, max, memory.begin() );
	uint hash = HashMurmur2(0xF00D, &memory[0], memory.size());

	watch.rangeBegin = min;
	watch.rangeEnd = max;
	watch.hash = hash;
}

//-----------------------------------//

bool PropertyPage::updateMemoryWatch(const Field* field, void* object)
{
	MemoryWatch watch;
	createMemoryWatch(field, object, watch);

	if(memoryWatches.find(field) == memoryWatches.end())
	{
		memoryWatches[field] = watch;
		return false;
	}

	const MemoryWatch& oldWatch = memoryWatches[field];

	if(oldWatch.hash == watch.hash)
		return false;

	watch.property = oldWatch.property;
	watch.object = oldWatch.object;

	// Returns true if the memory watch changed.
	memoryWatches[field] = watch;	
	return true;
}

//-----------------------------------//

bool PropertyPage::updateMemoryWatches()
{
	bool changed = false;

	MemoryWatchesMap::const_iterator it;
	for( it = memoryWatches.begin(); it != memoryWatches.end(); it++ )
	{
		const Field* field = it->first;
		
		MemoryWatch watch = memoryWatches[field];
		bool watchChanged = updateMemoryWatch(field, watch.object);

		if( !watchChanged ) continue;
		
		wxAny value = getFieldValue(field, watch.object);
		setPropertyValue(watch.property, value);

		changed = true;
	}

	return changed;
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif