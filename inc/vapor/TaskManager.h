/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/Subsystem.h"
#include "vapor/Singleton.h"
#include "vapor/Task.h"
#include "vapor/ConcurrentQueue.h"

namespace vapor {

//-----------------------------------//

/**
 * This event is sent when a task is either added to the manager, it is
 * picked up by a background thread and is processed, or when the thread
 * finishes processing of the task.
 */

struct VAPOR_API TaskEvent
{
	enum Enum
	{
		Added,
		Started,
		Finished,
	};

	TaskEvent::Enum event;
	TaskPtr task;
};

//-----------------------------------//

/** The number of threads that will be available for the thread pool 
 * to manage. If the size is too big, then the OS will be overloaded
 * managing a huge number of threads. If it is too small, then we run
 * the risk of sub-utilizing the machine resources. By default we will
 * use (numberOfCores-1) threads.
 */

const int DEFAULT_THREAD_POOL_SIZE = -1;

//-----------------------------------//

/**
 * Manages a group of tasks, running each one in a thread of execution 
 * managed by the operating system. These allow the engine to execute 
 * code concurrently, such as streaming content from disk, leaving the
 * main thread free to perform other tasks. Low-level synchhronization
 * support and OS abstraction are handled by the boost threads library.
 * A number of new threads are created at the task manager constructor
 * and tasks are run in the first idle thread found. This is generally
 * known as "Thread-pool pattern".
 */

class VAPOR_API TaskManager : public Subsystem
{
public:

	TaskManager( int size = DEFAULT_THREAD_POOL_SIZE );
	~TaskManager();

	// Adds a new task to be run by the sheduler.
	void addTask( const TaskPtr& task );

	// Sends the events to the observers.
	void update( double delta );

	// Notifies the observers of a given task event.
	fd::delegate< void(const TaskEvent&) > onTaskEvent;
	
protected:

	// Creates the thread pool.
	void createThreadPool( int poolSize );

	// Pushes a new event in the event queue.
	void pushEvent( TaskEvent::Enum event, const TaskPtr& task,
		bool sendEvent = false );

	// Runs the worker threads and executes the tasks.
	void runWorker();

	typedef std::vector<ThreadPtr> ThreadQueue;
	ThreadQueue threadPool;

	typedef concurrent_queue<TaskPtr> TaskQueue;
	TaskQueue tasks;

	typedef concurrent_queue<TaskEvent> EventQueue;
	EventQueue events;
};

//-----------------------------------//

} // end namespace