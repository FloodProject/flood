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

	Array<Thread*> threads; //!< threads assigned to taskpool
	ConcurrentQueue<Task*> tasks; //!< tasks for taskpool to execute
	ConcurrentQueue<TaskEvent> events; //!< task events
	Event1<TaskEvent> onTaskEvent; //!< task event delegate
	bool isStopping; //!< indicates when taskpool is shutting down

private:

	int threadCount;
	bool isWaiting;
};

//-----------------------------------//

NAMESPACE_CORE_END