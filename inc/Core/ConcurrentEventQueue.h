//////////////////////////////////////////////////////////////////////////
//
// Flood Engine © (2008-2013)
// <https://github.com/tritao/flood>
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventMap.h"
#include "ConcurrentQueue.h"

NAMESPACE_CORE_BEGIN

template < class KeyT, class EventPolicy >
class ConcurrentEventQueue : public EventMap<KeyT, EventPolicy>
{
private:
	// -- Types --
	typedef typename EventPolicy::storage_type	EventStorage;
	typedef std::pair<Key, EventStorage>		EventPair;
	typedef ConcurrentQueue<EventPair>			EventQueue;

private:
	// -- Members --
	EventQueue Events_;

public:
	ConcurrentEventQueue() {}
	virtual ~ConcurrentEventQueue()
	{
		EventPair ep;
		while(Events_.try_pop_front(ep))
			delete ep.second;
	}

	// -- Queue Events --
	template < typename T >
	void Push(Key key, T e)
	{
		Events_.push_back(std::make_pair(key, EventStorage(e)));
	}

	// -- Update / Process Events --
	void Update()
	{
		EventPair ep;
		while(Events_.try_pop_front(ep))
		{
			Fire( ep.first, ep.second );
			delete ep.second;
		}
	}
};

NAMESPACE_CORE_BEGIN