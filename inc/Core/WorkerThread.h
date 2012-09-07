//////////////////////////////////////////////////////////////////////////
//
// Author:  Corbin Hart
// Project: Genesis
// Date:    1/6/2011
//
// Worker - Wrapper for a boost::thread that consumes work from a pool of tasks.
//
// Copyright 2010-2011 (All rights reserved)
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/API.h"
#include "Core/Concurrency.h"

NAMESPACE_CORE_BEGIN

	// -- Forward Declarations --

class WorkerThreadPool;
class TaskBase;
class Completion;

// -- Class Definition --

class ALIGN_BEGIN(16) API_CORE WorkerThread
{
public:
	static const uint32 TaskCapacity = 256;

	// -- Members --
private:
	//  - task stuff
	WorkerThreadPool * Pool_;					// a pointer back to the Pool that owns *this
	std::vector<TaskBase *> Tasks_;				// an array of task pointers; this is accessed in the LIFO pattern
	Completion * CurrentCompletion_;			// the work that is currently being done

	// - thread stuff
	ThreadPtr Thread_;							// the actual thread object
	uint32 Index_;								// index inside the Pool
	MutexPtr TaskMutex_;						// mutex used to sync access to the Tasks_ array

public:
	static WorkerThread * This();				// static accessor for the current worker
	INLINE uint32 Index() const { return Index_; }

public:
	WorkerThread();
	~WorkerThread();

	// -- Init / Shutdown --

	// <summary>
	// Thread: MAIN
	// Called from the main thread. Creates the boost::thread, sets the index, etc etc. If the threadIndex == 0,
	//  then no boost::thread is created, instead it 
	// </summary>
	void Make( WorkerThreadPool * pool, uint32 threadIndex );

	// <summary>
	// Thread: MAIN
	// Called from the main thread. Joins the boost::thread, frees up any allocated memory, shuts down.
	// </summary>
	void Unmake();

	// -- Push / Wait --

	// <summary>
	// Thread: ANY
	// Called from any thread. NOTE: If the Pool has no work, tasks can only be pushed from the main thread. Adds a task
	//	to the top of the stack, or in the event the stack is full, run the task immediately.
	// </summary>
	bool Push( TaskBase * newTask );

	// <summary>
	// Thread: ANY
	// Called from any thread. Attempts to process tasks until the flag is set.
	// </summary>
	void YieldUntil( Completion * flag );

private:
	// -- Task Manipulation --

	// <summary>
	// Thread: THIS
	// Called by this thread. Attempts to spread work out over the pool if there is no set completion flag. If not, locks
	//	the internal mutex, pushes the task into the queue. Sets the task's completion flag to busy, sets the Pool's completion
	//	flag to the task's completion flag (if the Pool's flag is null), and wakes any idle workers.
	// </summary>
	bool AssignTask( TaskBase * newTask );

	// <summary>
	// Thread: THIS
	// Called by this thread. Locks the internal mutex (it's modifying the worker's state), attempts a partial pop of the task
	//	(to allow for other threads to potentially steal what's left) or just pops the whole task.
	// </summary>
	bool Pop( TaskBase *& outTask );

private:
	// <summary>
	// Thread: MAIN
	// This the main thread function. This runs in an infinite loop, waiting either for the shutdown signal, repeatedly
	//	calling Work(), or going into an idle state.
	//	
	// NOTE: Ignore the parameters, they were added to be made compatible with the vapor threading library
	// </summary>
	void Run(Thread *, void *);

	// <summary>
	// Thread: THIS
	// Attempts to pop a task off the stack and process it. If the pop fails, then it will attempt to steal work.
	// </summary>
	void Work( Completion * waitingFlag );

	// <summary>
	// Thread: THIS
	// Called by this thread when there is no more work in the internal queue, or in the task pool. Places the thread
	//	into an idle state awaiting wakeup from the Pool.
	// </summary>
	void Idle();

	// <summary>
	// Thread: THIS
	// Traverse the other threads, ask each one to copy half its work into this thread's task queue.
	// </summary>
	bool Steal();

	// <summary>
	// Thread: ANY
	// Called by a stealing thread. Attempts to give up half this thread's work, or split a last waiting task. Called by the thief.
	// </summary>
	bool GiveTo( WorkerThread * thief );

}	ALIGN_END(16);

NAMESPACE_CORE_END