/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/TaskManager.h"
#include "vapor/Utilities.h"

namespace vapor {

using namespace boost;

//-----------------------------------//

TaskManager::TaskManager( int poolSize )
{
	if( poolSize < 0 )
	{
		// By default use (numberOfCores-1) threads.
		poolSize = thread::hardware_concurrency()-1;

		// If boost returns 0, the number of threads is not available.
		if( poolSize <= 0 )
		{
			// Assume a "safe value" in this case.
			poolSize = 1;
		}
	}

	threadPool.resize( poolSize );

	foreach( ThreadPtr& thread, threadPool )
	{
		thread = new Thread(&TaskManager::runWorker, this);

		//#ifdef VAPOR_PLATFORM_WINDOWS
		//{
		//	static int i = 0;
		//	HANDLE th = thread->native_handle();
		//	SetThreadName( GetThreadId(th), "Task " + num_to_str(i++) );
		//}
		//#endif
	}

	info( "tasks", "Creating thread pool with %d thread(s)", poolSize );
}

//-----------------------------------//

TaskManager::~TaskManager()
{
	foreach( const ThreadPtr& thread, threadPool )
	{
		delete thread;
	} 
}

//-----------------------------------//

void TaskManager::addTask( const TaskPtr& task )
{
	if( !task ) return;

	// Check if the task is already assigned to run.
	if( tasks.find(task ) )
	{
		warn( "tasks", "Task is already in the queue" );
		return;
	}

	// Push event before the thread is pushed.
	pushEvent( TaskEvent::Added, task, true );

	// Push the task in the work queue.
	tasks.push( task );
}

//-----------------------------------//

class DelegateTask : Task
{
public:

	TaskDelegate task;

	void run()
	{
		if( !task.empty() )
		{
			task();
		}
	}
};

void TaskManager::addTask( const TaskDelegate& td )
{
	DelegateTask* dt = new DelegateTask();
	dt->task = td;

	addTask( TaskPtr(dt) );
}

//-----------------------------------//

void TaskManager::runWorker()
{
	forever
	{
		// Get a task and run it.
		TaskPtr task;
		tasks.wait_and_pop( task );

		if( task )
		{
			pushEvent( TaskEvent::Started, task );
			task->run();
			pushEvent( TaskEvent::Finished, task );
		}
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

void TaskManager::pushEvent( TaskEvent::Enum event, const TaskPtr& task,
							bool sendEvent )
{
	TaskEvent te;	
	te.event = event;
	te.task = task;

	if( sendEvent )
	{
		if( !onTaskEvent.empty() )
		{
			onTaskEvent( te );	
		}
	}
	else
	{
		events.push( te );
	}
}

//-----------------------------------//

} // end namespace