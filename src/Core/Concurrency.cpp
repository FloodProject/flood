/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Log.h"
#include "Core/Task.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

Thread::Thread()
	: handle(nullptr)
	, isRunning(false)
	, function(nullptr)
	, userdata(nullptr)
	, priority(ThreadPriority::Normal)
{
}

//-----------------------------------//

Thread::~Thread()
{
	pause();
}

//-----------------------------------//

Task::Task()
	: group(0)
	, priority(0)
	, userdata(nullptr)
{
}

//-----------------------------------//

void Task::run()
{
	callback(this);
}

//-----------------------------------//

typedef Vector<Thread*> ThreadQueue;

TaskPool::TaskPool(int8 size)
	: threadCount(size)
	, isWaiting(false)
	, isStopping(false)
{
	threads.Reserve(threadCount);

	for(size_t i = 0; i < (size_t) threadCount; i++)
	{
		Thread* thread = AllocateHeap(Thread);
		threads.Push(thread);

		ThreadFunction taskFunction;
		taskFunction.Bind(this, &TaskPool::run);
		
		thread->start(taskFunction, this);
		thread->setName("Task Pool");
	}

	LogInfo("Created task pool with '%d' threads", threads.Size());
}

//-----------------------------------//

TaskPool::~TaskPool()
{
	LogDebug("Destroying task pool");
	
	isStopping = true;

	for(auto thread : threads)
	{	
		Deallocate(thread);
	}
}

//-----------------------------------//

void TaskPool::pushEvent(Task* task, TaskState state)
{
	TaskEvent event;
	event.task = task;
	event.state = state;

	events.push_back(event);
	
	onTaskEvent(event);
}
//-----------------------------------//

void TaskPool::add(Task* task, uint8 priority)
{
	if (tasks.find(task))
	{
		LogAssert("Task is already in the queue");
		return;
	}

	if (priority > 0)
		tasks.push_front(task);
	else
		tasks.push_back(task);

	TaskEvent event;
	event.task = task;
	event.state = TaskState::Added;

	onTaskEvent(event);
}

//-----------------------------------//

void TaskPool::update()
{
	TaskEvent event;
	
	while (events.try_pop_front(event))
		onTaskEvent(event);
}

//-----------------------------------//

void TaskPool::run(Thread* thread, void* userdata)
{

	while (!isStopping && (!tasks.empty() || !isWaiting))
	{
		Task* task;
		tasks.wait_and_pop_front(task);

		if (!task) continue;

		pushEvent(task, TaskState::Started );

		task->run();
		
		pushEvent(task, TaskState::Finished );
	}
}

//-----------------------------------//

void TaskPool::waitAll()
{
	isWaiting = true;
	for(auto thread : threads)
		thread->join();
}

//-----------------------------------//

void TaskPool::restartThreads()
{

	isWaiting = false;
	for(auto thread : threads)
	{
		ThreadFunction taskFunction;
		taskFunction.Bind(this, &TaskPool::run);
		thread->start(taskFunction, this);
	}


}

//-----------------------------------//

NAMESPACE_CORE_END