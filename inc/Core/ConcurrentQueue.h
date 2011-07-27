/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <deque>
#include <algorithm>

NAMESPACE_CORE_BEGIN

//-----------------------------------//

struct Mutex;
struct Condition;

/**
 * Concurrent queue that is safe to use even with multiple producers
 * and consumers. This class is based from code located at website:
 * http://www.justsoftwaresolutions.co.uk/threading/implementing-a-
 * thread-safe-queue-using-condition-variables.html
 */

template<typename T>
class API_CORE ConcurrentQueue
{
public:

	ConcurrentQueue()
	{
		mutex = MutexCreate(AllocatorGetHeap());
		cond = ConditionCreate(AllocatorGetHeap());
	}

	//-----------------------------------//

	~ConcurrentQueue()
	{
		MutexDestroy(mutex);
		ConditionDestroy(cond);
	}

	//-----------------------------------//

	void push(const T& value)
	{
		MutexLock(mutex);
		queue.push_back(value);
		MutexUnlock(mutex);
		ConditionWakeOne(cond);
	}

	//-----------------------------------//

	bool empty() const
	{
		MutexLock(mutex);
		bool empty = queue.empty();
		MutexUnlock(mutex);
		
		return empty;
	}

	//-----------------------------------//

	bool try_pop(T& popped_value)
	{
		MutexLock(mutex);
	    
		if( queue.empty() )
			return false;
	    
		popped_value = queue.front();
		queue.pop_front();
		
		MutexUnlock(mutex);

		return true;
	}

	//-----------------------------------//

	void wait_and_pop(T& popped_value)
	{
		MutexLock(mutex);
	
		while( queue.empty() )
			ConditionWait(cond, mutex);
	
		popped_value = queue.front();
		queue.pop_front();

		MutexUnlock(mutex);
	}

	//-----------------------------------//

	bool find(const T& value)
	{
		MutexLock(mutex);

		typename std::deque<T>::const_iterator it;
		it = std::find(queue.begin(), queue.end(), value);
		bool found = it != queue.end();

		MutexUnlock(mutex);

		return found;
	}

	//-----------------------------------//

protected:

	std::deque<T> queue;

	Mutex* mutex;
	Condition* cond;
};

//-----------------------------------//

NAMESPACE_CORE_END