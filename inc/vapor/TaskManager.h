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

/**
 * Manages a group of tasks that get run in separate threads.
 * These allow the engine to execute code concurrently, such as 
 * streaming content from disk, leaving the main thread free to
 * perform other tasks. We spawn a few threads in a thread pool
 * and re-use them for tasks. Tasks get executed in idle threads.
 */

class VAPOR_API TaskManager : public Subsystem
{
public:

	TaskManager( int size = -1 );
	~TaskManager();

	// Adds a new task to be run by the sheduler.
	void addTask( const TaskPtr& task );

	// Gets the worker threads.
	GETTER(Threads, const std::vector<Thread*>&, threadPool)

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

	// Runs in the worker threads and executes the tasks.
	void runWorker();

	typedef std::vector<Thread*> ThreadQueue;
	ThreadQueue threadPool;

	typedef concurrent_queue<TaskPtr> TaskQueue;
	TaskQueue tasks;

	typedef concurrent_queue<TaskEvent> TaskEventQueue;
	TaskEventQueue events;
};

//-----------------------------------//

} // end namespace