//////////////////////////////////////////////////////////////////////////
//
// Flood Engine © (2008-2013)
// <https://github.com/tritao/flood>
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/EventMap.h"
#include "Core/Concurrency.h"
#include "Core/ConcurrentEventQueue.h"

NAMESPACE_CORE_BEGIN

	class Actor;

	template < typename T >
	class ActorMessage : public EventBase
	{
	public:
		Actor * Sender;
		T Data;
	};

	class Actor
	{
		// -- Types --
	public:
		typedef ConcurrentEventQueue<std::string, BaseClassEventPolicy> Mailbox;

	protected:
		Mailbox Mailbox_;
		Mutex * Mutex_;

	public:
		
		Actor() {}
		virtual ~Actor() {}

		void Receive()
		{
			MutexLock(Mutex_);

			try
			{
				Mailbox_.Update();
			}
			catch(...)
			{
				LogError("Caught an unhandled exception.");
			}

			MutexUnlock(Mutex_);
		}

		template < class DataT >
		void Send( Actor * receiver, std::string const & key, DataT messageData )
		{
			ActorMessage<DataT> * msg = AllocateHeap(ActorMessage);
			msg->Sender = this;
			msg->Data = messageData;

			receiver->Mailbox_.Push(key, msg);
		}
	};

	Actor * ActorSpawn();
	void ActorDespawn(Actor * actor);

NAMESPACE_CORE_END