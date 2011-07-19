/************************************************************************
*
* vapor3D Server © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Memory.h"
#include "Core/Log.h"

NAMESPACE_BEGIN

//-----------------------------------//

Thread* ThreadCreate(Allocator* alloc)
{
	Thread* thread = Allocate(Thread, alloc);

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

Mutex* MutexCreate(Allocator* alloc)
{
	Mutex* mutex = Allocate(Mutex, alloc);
	MutexInit(mutex);

	return mutex;
}

//-----------------------------------//

void MutexDestroy(Mutex* mutex)
{
	Deallocate(mutex);
}

//-----------------------------------//

Condition* ConditionCreate(Allocator* alloc)
{
	Condition* cond = Allocate(Condition, alloc);
	ConditionInit(cond);

	return cond;
}

//-----------------------------------//

void ConditionDestroy(Condition* cond)
{
	Deallocate( cond);
}

//-----------------------------------//

Task* TaskCreate(Allocator* alloc)
{
	Task* task = Allocate(Task, alloc);
	
	task->Group = 0;
	task->Priority = 0;
	task->Userdata = nullptr;
	
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
	task->Callback(task);
	TaskDestroy(task);
}

//-----------------------------------//

static void TaskPoolRun(Thread*, void*);
typedef std::vector<Thread*> ThreadQueue;

TaskPool* TaskPoolCreate(Allocator* alloc, int8 Size)
{
	TaskPool* pool = Allocate(TaskPool, alloc);
	
	pool->IsStopping = false;

	ThreadQueue& threads = pool->Threads;
	threads.reserve(Size);

	for( size_t i = 0; i < (size_t) Size; i++ )
	{
		Thread* thread = ThreadCreate(alloc);
		threads.push_back(thread);
		
		ThreadStart(thread, TaskPoolRun, pool);
		ThreadSetName(thread, "Task Pool");
	}

	LogInfo("Created task pool with '%d' threads", threads.size());

	return pool;
}

//-----------------------------------//

void TaskPoolDestroy(TaskPool* pool)
{
	if( !pool ) return;
	
	pool->IsStopping = true;

	ThreadQueue& threads = pool->Threads;

	for( size_t i = 0; i < threads.size(); i++ )
	{
		Thread* thread = threads[i];
		ThreadDestroy(thread);
	}

	Deallocate<TaskPool>(pool);
}

//-----------------------------------//

static void TaskPoolPushEvent( TaskPool* pool, Task* task, TaskState state)
{
	TaskEvent event;
	event.task = task;
	event.state = state;

	pool->Events.push( event);
}

//-----------------------------------//

void TaskPoolAdd(TaskPool* pool, Task* task)
{
	assert( task && "Invalid task" );
	assert( !pool->Tasks.find(task) && "Task is already in the queue" );

	LogDebug("Added task to pool");

	pool->Tasks.push(task);

	TaskEvent event;
	event.task = task;
	event.state = TaskState::Added;

	pool->OnTaskEvent(event);
}

//-----------------------------------//

void TaskPoolUpdate(TaskPool* pool)
{
	TaskEvent event;
	
	while( pool->Events.try_pop(event) )
		pool->OnTaskEvent(event);
}

//-----------------------------------//

static void TaskPoolRun(Thread* thread, void* userdata)
{
	TaskPool* pool = (TaskPool*) userdata;

	while( !pool->IsStopping )
	{
		Task* task;
		pool->Tasks.wait_and_pop(task);

		if( !task ) continue;

		TaskPoolPushEvent( pool, task, TaskState::Started );
		TaskRun(task);
		LogDebug("After task run!");
		TaskPoolPushEvent( pool, task, TaskState::Finished );

		LogDebug("THIS SHOULD PRINT");
	}
}

//-----------------------------------//

NAMESPACE_END