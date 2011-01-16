/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PropertyPage.h"
#include "math/Hash.h"

namespace vapor { namespace editor {

//-----------------------------------//

void PropertyPage::createMemoryWatch(const Class* type, void* object, MemoryWatch& watch)
{
	const FieldsMap& fields = type->getFields();
	
	if( fields.empty() )
		return;
	
	byte* min = (byte*) 0xffffffff;
	byte* max = nullptr;

	FieldsMap::const_iterator it;
	for(it = fields.cbegin(); it != fields.cend(); ++it)
	{
		int offset = it->second->offset;
		byte* field = (byte*) object + offset;
		
		if( field < min ) min = field;
		if( field > max ) max = field;
	}

	std::vector<byte> memory;
	memory.resize(max - min);
	
	std::copy( min, max, memory.begin() );
	uint hash = Hash::Murmur2(memory, 0xF00D);

	watch.rangeBegin = min;
	watch.rangeEnd = max;
	watch.hash = hash;
}

//-----------------------------------//

bool PropertyPage::updateMemoryWatch(const Class* type, void* object)
{
	MemoryWatch watch;
	createMemoryWatch(type, object, watch);

	if(memoryWatches.find(type) == memoryWatches.end())
	{
		memoryWatches[type] = watch;
		return false;
	}

	const MemoryWatch& oldWatch = memoryWatches[type];

	if(oldWatch.hash == watch.hash)
		return false;

	// Returns true if the memory watch changed.
	memoryWatches[type] = watch;	
	return true;
}

//-----------------------------------//

bool PropertyPage::updateMemoryWatches()
{
	bool changed = false;

	const EntityPtr& entity = selectedEntity.lock();
	
	if( !entity )
		return false;

	const ComponentMap& components = entity->getComponents();
	
	ComponentMap::const_iterator it;
	for(it = components.cbegin(); it != components.cend(); ++it)
	{
		const Class* type = it->first;
		void* object = it->second.get();

		bool watchChanged = updateMemoryWatch(type, object);
		
		if( !changed )
			changed = watchChanged;
	}

	return changed;
}

//-----------------------------------//

} } // end namespaces