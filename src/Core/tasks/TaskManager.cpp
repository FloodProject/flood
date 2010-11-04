/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/TaskManager.h"
#include "vapor/Utilities.h"

namespace vapor {

//-----------------------------------//

TaskManager::TaskManager( int poolSize )
{
#ifdef VAPOR_THREADING
	createThreadPool( poolSize );
#else
	createThreadPool( 0 );
#endif
}

//-----------------------------------//

TaskManager::~TaskManager()
{
	foreach( Thread* thread, threadPool )
	{
		THREAD(delete thread;)
	}
}

//-----------------------------------//

void TaskManager::createThreadPool( int poolSize )
{
#ifdef VAPOR_THREADING
	// By default use (numCPUCores-1) threads.
	if( poolSize < 0 )
		poolSize = boost::thread::hardware_concurrency()-1;

	threadPool.resize( poolSize );

	foreach( Thread* thread, threadPool )
	{
		thread = new Thread(&TaskManager::runWorker, this);
	}
#endif

	Log::info( "Created thread pool with %d thread(s)", poolSize );
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

void TaskManager::update( double )
{
	TaskEvent event;
	
	while( events.try_pop(event) )
	{
		if( !onTaskEvent.empty() )
		{
			onTaskEvent( event );	
		}
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
	{
		if( !onTaskEvent.empty() )
		{
			onTaskEvent( taskEvent );	
		}
	}
	else
	{
		events.push( taskEvent );
	}
}

//-----------------------------------//

} // end namespace