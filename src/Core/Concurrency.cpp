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

namespace vapor {

//-----------------------------------//

Thread* ThreadCreate(Allocator* mem)
{
	Thread* thread = Allocate<Thread>(mem);

	if( !thread ) return nullptr;

	thread->Handle = nullptr;
	thread->IsRunning = false;
	thread->Function = nullptr;
	thread->Userdata = nullptr;
	thread->Priority = ThreadPriority::Normal;

	return thread;
}

//-----------------------------------//

void ThreadDestroy(Thread* thread, Allocator* mem)
{
	ThreadStop(thread);
	Deallocate(mem, thread);
}

//-----------------------------------//

Mutex* MutexCreate(Allocator* mem)
{
	Mutex* mutex = Allocate<Mutex>(mem);
	MutexInit(mutex);

	return mutex;
}

//-----------------------------------//

void MutexDestroy(Mutex* mutex, Allocator* mem)
{
	Deallocate(mem, mutex);
}

//-----------------------------------//

Condition* ConditionCreate(Allocator* mem)
{
	Condition* cond = Allocate<Condition>(mem);
	ConditionInit(cond);

	return cond;
}

//-----------------------------------//

void ConditionDestroy(Condition* cond, Allocator* mem)
{
	Deallocate(mem, cond);
}

//-----------------------------------//

Task* TaskCreate(Allocator* mem)
{
	Task* task = Allocate<Task>(mem);
	return task;
}

//-----------------------------------//

void TaskDestroy(Task* task, Allocator* mem)
{
	Deallocate(mem, task);
}

//-----------------------------------//

void TaskRun(Task* task)
{
	if( !task ) return;
	task->Callback(task);
	TaskDestroy(task, AllocatorGetHeap());
}

//-----------------------------------//

static void TaskPoolRun(void*);
typedef std::vector<Thread*> ThreadQueue;

TaskPool* TaskPoolCreate(Allocator* mem, int8 Size)
{
	TaskPool* pool = Allocate<TaskPool>(mem);
	
	pool->IsStopping = false;

	ThreadQueue& threads = pool->Threads;
	threads.reserve(Size);

	for( size_t i = 0; i < (uint8) Size; i++ )
	{
		Thread* thread = ThreadCreate(mem);
		threads.push_back(thread);
		
		ThreadStart(thread, TaskPoolRun, pool);
		ThreadSetName(thread, "Task Pool");
	}

	//LogDebug("Task pool created with %d threads", threads.size());

	return pool;
}

//-----------------------------------//

void TaskPoolDestroy(TaskPool* pool, Allocator* mem)
{
	if( !pool ) return;
	
	pool->IsStopping = true;

	ThreadQueue& threads = pool->Threads;

	for( size_t i = 0; i < threads.size(); i++ )
	{
		Thread* thread = threads[i];
		ThreadDestroy(thread, mem);
	}

	Deallocate<TaskPool>(mem, pool);
}

//-----------------------------------//

static void TaskPoolPushEvent( TaskPool* pool, Task* task, TaskEvent::Enum which)
{
	TaskEvent event;
	event.task = task;
	event.event = which;

	pool->Events.push( event);
}

//-----------------------------------//

void TaskPoolAdd(TaskPool* pool, Task* task)
{
	assert( task && "Invalid task" );
	assert( !pool->Tasks.find(task) && "Task is already in the queue" );

	pool->Tasks.push(task);

	TaskEvent event;
	event.task = task;
	event.event = TaskEvent::Added;

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

static void TaskPoolRun(void* userdata)
{
	TaskPool* pool = (TaskPool*) userdata;

	while( !pool->IsStopping )
	{
		Task* task;
		pool->Tasks.wait_and_pop(task);

		if( !task ) continue;

		TaskPoolPushEvent( pool, task, TaskEvent::Started );
		TaskRun(task);
		TaskPoolPushEvent( pool, task, TaskEvent::Finished );

		TaskDestroy(task, AllocatorGetHeap());
	}
}

//-----------------------------------//

} // end namespace