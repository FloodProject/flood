/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Subsystem.h"
#include "ConcurrentQueue.h"
#include "Event.h"
#include "ReferenceCount.h"

FWD_DECL_INTRUSIVE(Task)

namespace vapor {

//-----------------------------------//

/**
 * This event is sent when a task is either added to the manager, it is
 * picked up by a background thread and is processed, or when the thread
 * finishes processing of the task.
 */

struct CORE_API TaskEvent
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

/**
 * Manages a group of tasks that get run in separate threads.
 * These allow the engine to execute code concurrently, such as 
 * streaming content from disk, leaving the main thread free to
 * perform other tasks. We spawn a few threads in a thread pool
 * and re-use them for tasks. Tasks get executed in idle threads.
 */

class CORE_API TaskManager : public Subsystem
{
public:

	TaskManager( int numThreads = -1 );
	~TaskManager();

	// Adds a new task to be run by the sheduler.
	void addTask( const TaskPtr& task );

	// Gets the worker threads.
	GETTER(Threads, const std::vector<Thread*>&, threads)

	// Sends the events to the observers.
	void update( double delta );

	// Notifies the observers of a given task event.
	Event1<const TaskEvent&> onTaskEvent;
	
protected:

	// Creates the threads in the pool.
	void createThreads( int numThreads );

	// Pushes a new event in the event queue.
	void pushEvent( TaskEvent::Enum, const TaskPtr&, bool sendEvent = false );

	// Runs in the worker threads and executes the tasks.
	void runWorker();

	typedef std::vector<Thread*> ThreadQueue;
	ThreadQueue threads;

	typedef ConcurrentQueue<TaskPtr> TaskQueue;
	TaskQueue tasks;

	typedef ConcurrentQueue<TaskEvent> TaskEventQueue;
	TaskEventQueue events;
};

//-----------------------------------//

} // end namespace