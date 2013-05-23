/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Thread* ThreadCreate(Allocator* alloc)
{
	Thread* thread = Allocate(alloc, Thread);

	if( !thread ) return nullptr;

	thread->Handle = nullptr;
	thread->IsRunning = false;
	thread->Function = nullptr;
	thread->Userdata = nullptr;
	thread->Priority = ThreadPriority::Normal;

	return thread;
}

//-----------------------------------//

void ThreadDestroy(Thread* thread)
{
	ThreadPause(thread);
	Deallocate(thread);
}

//-----------------------------------//

TaskPool::TaskPool()
	: Threads(*AllocatorGetHeap())
{}

//-----------------------------------//

Task* TaskCreate(Allocator* alloc)
{
	Task* task = Allocate(alloc, Task);
	
	task->group = 0;
	task->priority = 0;
	task->userdata = nullptr;
	
	return task;
}

//-----------------------------------//

void TaskDestroy(Task* task)
{
	Deallocate(task);
}

//-----------------------------------//

void TaskRun(Task* task)
{
	if( !task ) return;
	task->callback(task);
	TaskDestroy(task);
}

//-----------------------------------//

static void TaskPoolRun(Thread*, void*);
typedef Array<Thread*> ThreadQueue;

TaskPool* TaskPoolCreate(Allocator* alloc, int8 Size)
{
	TaskPool* pool = Allocate(alloc, TaskPool);
	
	pool->IsStopping = false;

	ThreadQueue& threads = pool->Threads;
	array::reserve(threads, Size);

	for( size_t i = 0; i < (size_t) Size; i++ )
	{
		Thread* thread = ThreadCreate(alloc);
		array::push_back(threads, thread);

		ThreadFunction taskFunction;
		taskFunction.Bind(TaskPoolRun);
		
		ThreadStart(thread, taskFunction, pool);
		ThreadSetName(thread, "Task Pool");
	}

	LogInfo("Created task pool with '%d' threads", array::size(threads));

	return pool;
}

//-----------------------------------//

void TaskPoolDestroy(TaskPool* pool)
{
	if( !pool ) return;

	LogDebug("Destroying task pool");
	
	pool->IsStopping = true;

	ThreadQueue& threads = pool->Threads;

	for( size_t i = 0; i < array::size(threads); ++i)
	{
		Thread* thread = threads[i];
		ThreadDestroy(thread);
	}

	Deallocate(pool);
}

//-----------------------------------//
#ifdef ENABLE_TASK_EVENTS
static void TaskPoolPushEvent( TaskPool* pool, Task* task, TaskState state)
{
	TaskEvent event;
	event.task = task;
	event.state = state;

	pool->Events.push( event);
}
#endif
//-----------------------------------//

void TaskPoolAdd(TaskPool* pool, Task* task, uint8 priority)
{
	if( !task ) return;

	if( pool->Tasks.find(task) )
	{
		LogAssert("Task is already in the queue");
		return;
	}

	if(priority > 0)
		pool->Tasks.push_front(task);
	else
		pool->Tasks.push_back(task);

#ifdef ENABLE_TASK_EVENTS
	TaskEvent event;
	event.task = task;
	event.state = TaskState::Added;

	pool->OnTaskEvent(event);
#endif
}

//-----------------------------------//

void TaskPoolUpdate(TaskPool* pool)
{
	TaskEvent event;
	
	while( pool->Events.try_pop_front(event) )
		pool->OnTaskEvent(event);
}

//-----------------------------------//

static void TaskPoolRun(Thread* thread, void* userdata)
{
	TaskPool* pool = (TaskPool*) userdata;

	while( !pool->IsStopping )
	{
		Task* task;
		pool->Tasks.wait_and_pop_front(task);

		if( !task ) continue;

#ifdef ENABLE_TASK_EVENTS
		TaskPoolPushEvent( pool, task, TaskState::Started );
#endif

		TaskRun(task);
		
#ifdef ENABLE_TASK_EVENTS
		TaskPoolPushEvent( pool, task, TaskState::Finished );
#endif
	}
}

//-----------------------------------//

NAMESPACE_CORE_END