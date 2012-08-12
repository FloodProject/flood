//////////////////////////////////////////////////////////////////////////
//
// Author:  Corbin Hart
// Project: Genesis
// Date:    1/14/2011
//
// Associator - The primary management point of the relationship between functions
//	the events upon which they depend. It provides basic functionality for the
//	binding of functions to events. This handles only the instant invocation of
//	events.
//
//	Associator::EventPolicy - This is to wrap the way that the event type is up-cast
//	from its base type. This functionality will be utilized through the interface
//	EventPolicy::event_cast<dest_type>(base_type)
//
// Link - Class that manages a list of events, queuing and processing them. This is
//	intended for single-threaded use. It is meant to be extended later for use in a
//	multi-threaded manner.
//
// Copyright 2010-2011 (All rights reserved)
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/API.h"
#include <functional>
#include <unordered_map>
#include <forward_list>

NAMESPACE_CORE_BEGIN

template < class KeyT, class EventPolicy >
class EventMap : private EventPolicy
{
public:
	// -- Types --
	typedef KeyT							Key;	// <- NOTE: Key should be a movable type, or a pod
	typedef typename EventPolicy::base_type	Event;

private:
	typedef typename EventPolicy::ref_type				EventParam;
	typedef std::function<void (EventParam)>			Callable;			// - callback type that is contained in the function table
	typedef std::list<Callable>							CallableSequence;	// - individual entry into the table
	typedef std::unordered_map<Key, CallableSequence>	CallableMapping;	// - a map of event types to lists of callbacks

public:
	// <summary>
	// The Tag class acts as an encapsulated iterator that allows an external
	//	system to safely remove their registered event functions from an the
	//	internal lists. It is safe to copy around. It also will not deregister upon
	//	destruction, which means that you can safely ignore it after calling Associator::Bind.
	//	It should be noted, however, that if a Tag is not stored for later use, there is no way
	//	to remove a registered callback, as the Associator generates binding lambdas.
	// </summary>
	class Tag
	{
	private:
		CallableSequence * Seq_;
		typename CallableSequence::const_iterator Iter_;

	public:
		Tag() : Seq_(nullptr) {}
		Tag( CallableSequence * seq, typename CallableSequence::const_iterator iter) : Seq_(seq), Iter_(iter) {}
		void Shutdown() { Seq_->erase(Iter_); }
	};

	typedef std::forward_list<Tag> TagList;

private:
	// -- Members --
	CallableMapping CallableMap_;

public:
	// -- Constructor / Destructor --
	EventMap() {}
	virtual ~EventMap() { CallableMap_.clear(); }

	// -- Binding Behavior --
	template < typename T >
	Tag Bind( Key key, std::function<void (T)> func )	// <- NOTE: std::function is movable, passing by value should enable the move constructor
	{

		// create the casting adapter
		Callable f = [&, func] ( EventParam e ) -> void
			{
				func( this->event_cast<T>(e) );
			};

		return Bind(key, f);
	}

	template <>
	Tag Bind( Key key, std::function<void (EventParam)> func )				// <- NOTE: std::function is movable
	{
		// insert it into the function table
		CallableSequence & fl = CallableMap_[key];
		fl.push_back( func );

		return ( Tag(&fl, --fl.end()) );
	}

	// -- Fire Events --
	template < typename T >
	void Fire( Key key, T e )
	{
		CallableSequence const & fl = CallableMap_[key];
		for(auto f = fl.begin(); f != fl.end(); ++f)
			(*f)(EventParam(e));
	}
};

template < class KeyT, class EventPolicy >
class EventQueue : public EventMap<KeyT, EventPolicy>
{
private:
	// -- Types --
	typedef typename EventPolicy::storage_type	EventStorage;
	typedef std::pair<Key, EventStorage>	EventPair;
	typedef std::vector<EventPair>			EventList;

private:
	// -- Members --
	EventList Events_;

public:
	EventQueue() {}
	virtual ~EventQueue()
	{
		for(auto ep = Events_.begin(); ep != Events_.end(); ++ep)
			delete ep->second;
		Events_.clear();
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
		for(auto ep = Events_.begin(); ep != Events_.end(); ++ep)
		{
			Fire( ep->first, ep->second );
			delete ep->second;
		}

		Events_.clear();
	}
};

class EventBase {};

class BaseClassEventPolicy
{
public:
	typedef EventBase			base_type;
	typedef	EventBase const *	ref_type;
	typedef EventBase *			storage_type;

	template < typename T >
	static T event_cast(ref_type e) { return static_cast<T>(e); }
};

NAMESPACE_CORE_END