/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "TaskManager.h"
#include "Task.h"
#include "Log.h"

namespace vapor {

//-----------------------------------//

TaskManager::TaskManager( int numThreads )
{
#ifdef VAPOR_THREADING
	createThreads(numThreads);
#else
	createThreads(0);
#endif
}

//-----------------------------------//

TaskManager::~TaskManager()
{
	for( uint i = 0; i < threads.size(); i++ )
	{
		Thread* thread = threads[i];
		THREAD(delete thread;)
	}
}

//-----------------------------------//

void TaskManager::createThreads( int numThreads )
{
#ifdef VAPOR_THREADING
	// By default use (numCPUCores-1) threads.
	if( numThreads < 0 )
		numThreads = boost::thread::hardware_concurrency()-1;

	threads.resize( numThreads );

	foreach( Thread* thread, threads )
	{
		thread = new Thread(&TaskManager::runWorker, this);
	}
#endif

	Log::info( "Created thread pool with %d thread(s)", numThreads );
}

//-----------------------------------//

void TaskManager::addTask( const TaskPtr& task )
{
	assert( task && "Invalid task" );

	// Check if the task is already assigned to run.
	assert( !tasks.find(task) && "Task is already in the queue" );

	// Push event before the thread is pushed.
	pushEvent( TaskEvent::Added, task, true );

#ifdef VAPOR_THREADING
	// Push the task in the work queue.
	tasks.push( task );
#else
	task->run();
#endif
}

//-----------------------------------//

void TaskManager::runWorker()
{
	while(true)
	{
		// Get a task and run it.
		TaskPtr task;
		tasks.wait_and_pop( task );

		if( !task )
			continue;

		pushEvent( TaskEvent::Started, task );
		task->run();
		pushEvent( TaskEvent::Finished, task );
	}
}

//-----------------------------------//

void TaskManager::update( float )
{
	TaskEvent event;
	
	while( events.try_pop(event) )
	{
		onTaskEvent( event );	
	}
}

//-----------------------------------//

void TaskManager::pushEvent( TaskEvent::Enum event,
							 const TaskPtr& task, bool sendEvent )
{
	TaskEvent taskEvent;	
	taskEvent.event = event;
	taskEvent.task = task;

	if( sendEvent )
		onTaskEvent( taskEvent );	
	else
		events.push( taskEvent );
}

//-----------------------------------//

} // end namespace