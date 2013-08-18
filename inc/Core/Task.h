/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Concurrency.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

/**
 * Tasks provide an higher level interface to concurrency than threads.
 * They can be managed by the engine and grouped in different hardware
 * threads.
 */

class Task;
typedef Delegate1<Task*> TaskFunction;

class API_CORE Task
{
public:
	Task();

	/**
	 * Run task function
	 */
	void run();

	int16 group;
	int16 priority; //!< task priority
	TaskFunction callback; //!< task function
	void* userdata; //!< task function arguments
};



enum class TaskState
{
	Added,
	Started,
	Finished
};

struct API_CORE TaskEvent
{
	Task* task;
	TaskState state;
};

#include "Core/ConcurrentQueue.h"

class API_CORE TaskPool
{
public:

	/**
	 * Create taskpool.
	 * @param size number of threads the taskpool uses 
	 */
	TaskPool(int8 size);
	
	~TaskPool();

	/**
	 * Adds task to taskpool.
	 * @param task task to add
	 * @param priority priority of task to add, 0 if not to be prioritized, higher than zero otherwise
	 */
	void add(Task* task, uint8 priority);


	/**
	 * Waits on all threads to terminate.
	 */
	void waitAll();

	/**
	 * Restart threads.
	 */
	void restartThreads();

	/**
	 * Raise task event delegates.
	 */
	void update();

private:

	/**
	 * Run taskpool in a thread.
	 * @param thread thread to run taskpool in
	 * @param userdata taskpool to run thread in
	 */
	void run(Thread* thread, void* userdata);

	/**
	 * Store task event
	 * @param task task that triggered the event
	 * @param state event descripiton
	 */
	void pushEvent(Task* task, TaskState state);

public:

	std::vector<Thread*> threads; //!< threads assigned to taskpool
	ConcurrentQueue<Task*> tasks; //!< tasks for taskpool to execute
	ConcurrentQueue<TaskEvent> events; //!< task events
	Event1<TaskEvent> onTaskEvent; //!< task event delegate
	bool isStopping; //!< indicates when taskpool is shutting down

private:

	int threadCount;
	bool isWaiting;
};

//-----------------------------------//


	// <summary>
	// This class represents a completion flag. It is a simple wrapper around an
	// atomic integer.
	// </summary>
	class API_CORE Completion
	{
	private:

		Atomic<uint32> status;

	public:
		Completion() : status(0) {}

		INLINE bool Done() { return status.read() == 0; }
		INLINE void Set(bool isBusy)
		{
			if(isBusy)
				status.increment();
			else
				status.decrement();
		}
	};

	// <summary>
	// Class wrapping the interface to actually do work. Contains functions to allow for more complicated behavior, such as
	//	stealing and spreading.
	// </summary>
	
	
	class ALIGN_BEGIN(16) API_CORE TaskBase
	{
	public:
		typedef std::vector<TaskBase *> Range;

	public:
		Completion * Completion_;

	public:
		TaskBase( Completion * comp )
			: Completion_(comp)
		{}
		virtual ~TaskBase() {}	// NOTE: DOES NOT DELETE COMPLETION_

		// <summary>
		// Where the actual work is done. Fill this out.
		// </summary>
		virtual void Do() = 0;

		// <summary>
		// Allocates new tasks and sub-divides the work based on data-parallelism, using a provided target granularity.
		// </summary>
		INLINE virtual Range * Split( uint32 numSlices ) { return nullptr; }

		// <summary>
		// Sub-divides the work, if possible, with a granularity of 1. This is intended to keep as much of the work as
		//	possible in the Worker thread's task queue, providing an opportunity for other worker threads to further steal
		//	from the work.
		// </summary>
		INLINE virtual bool  Slice( TaskBase *& outTask ) { return false; }
	}	ALIGN_END(16);

NAMESPACE_CORE_END