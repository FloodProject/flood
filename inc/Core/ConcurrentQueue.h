/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Memory.h"
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

template<typename T> class ConcurrentQueue
{
public:

	void push_front(const T& value)
	{
		mutex.lock();
		queue.push_front(value);
		mutex.unlock();
		
		condition.wakeOne();
	}

	//-----------------------------------//

	void push_back(const T& value)
	{
		mutex.lock();
		queue.push_back(value);
		mutex.unlock();
		
		condition.wakeOne();
	}

	//-----------------------------------//

	bool empty() const
	{
		mutex.lock();
		bool empty = queue.empty();
		mutex.unlock();
		
		return empty;
	}

	//-----------------------------------//

	bool try_pop_front(T& popped_value)
	{
		mutex.lock();

		if( queue.empty() )
		{
			mutex.unlock();
			return false;
		}

		popped_value = queue.front();
		queue.pop_front();
		
		mutex.unlock();

		return true;
	}

	//-----------------------------------//

	void wait_and_pop_front(T& popped_value)
	{
		mutex.lock();

		while( queue.empty() )
			condition.wait(mutex);
	
		popped_value = queue.front();
		queue.pop_front();

		mutex.unlock();
	}

	//-----------------------------------//

	bool find(const T& value)
	{
		mutex.lock();

		typename std::deque<T>::const_iterator it;
		it = std::find(queue.begin(), queue.end(), value);
		bool found = it != queue.end();

		mutex.unlock();

		return found;
	}

	//-----------------------------------//

protected:

	std::deque<T> queue;

	mutable Mutex mutex;
	Condition condition;
};

//-----------------------------------//

NAMESPACE_CORE_END