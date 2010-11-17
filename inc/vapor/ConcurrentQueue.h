/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Concurrent queue that is safe to use even with multiple producers
 * and consumers. This class is based from code located at website:
 * http://www.justsoftwaresolutions.co.uk/threading/implementing-a-
 * thread-safe-queue-using-condition-variables.html
 */

template<typename T>
class VAPOR_API concurrent_queue
{
public:

	void push(const T& value)
	{
		THREAD(boost::mutex::scoped_lock lock(mutex);)
		
		queue.push_back(value);
		THREAD(lock.unlock();)

		THREAD(cond_var.notify_one();)
	}

	//-----------------------------------//

	bool empty() const
	{
		THREAD(boost::mutex::scoped_lock lock(mutex);)
		
		return queue.empty();
	}

	//-----------------------------------//

	bool try_pop(T& popped_value)
	{
		THREAD(boost::mutex::scoped_lock lock(mutex);)
	    
		if( queue.empty() )
			return false;
	    
		popped_value = queue.front();
		queue.pop_front();
		
		return true;
	}

	//-----------------------------------//

	void wait_and_pop(T& popped_value)
	{
		THREAD(boost::mutex::scoped_lock lock(mutex);)
	    
		while( queue.empty() )
			THREAD(cond_var.wait(lock);)
	    
		popped_value = queue.front();
		queue.pop_front();
	}

	//-----------------------------------//

	bool find(const T& value)
	{
		THREAD(boost::mutex::scoped_lock lock(mutex);)

		std::deque<T>::const_iterator it;
		it = std::find(queue.begin(), queue.end(), value);
		
		if( it != queue.end() )
			return true;

		return false;
	}

	//-----------------------------------//

protected:

	std::deque<T> queue;

	THREAD(mutable boost::mutex mutex;)
	THREAD(boost::condition_variable cond_var;)
};

//-----------------------------------//

} // end namespace