/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Memory.h"
#include <UnitTest++.h>

namespace {

static int value = 20;

static void Run(Thread*, void* data)
{
	int* num = (int*) data;
	value = *num;
}

static Condition* cond;
static Mutex* mutex;

static void Run2(Thread*, void* data)
{
	value = 30;
	ConditionWakeOne(cond);
}
}

SUITE(Core)
{
	TEST(ThreadBasics)
	{
		ThreadPtr thread( pThreadCreate(AllocatorGetHeap()) );

		ThreadFunction fn;
		fn.Bind(::Run);

		int data = 42;
		ThreadStart(thread, fn, &data);
		ThreadJoin(thread);

		CHECK(42 == value);
	}

	TEST(ThreadConditions)
	{
		value = 20;

		ThreadPtr thread( pThreadCreate(AllocatorGetHeap()) );

		cond = ConditionCreate( AllocatorGetHeap() );
		mutex = MutexCreate( AllocatorGetHeap() );

		ThreadFunction fn;
		fn.Bind(Run2);

		ThreadStart(thread, fn, nullptr);
		CHECK(20 == value);

		MutexLock(mutex);

		// Prevents deadlock.
		while(value == 20)
		{
			ConditionWait(cond, mutex);
		}

		MutexUnlock(mutex);

		ThreadJoin(thread);
		CHECK( 30 == value);

		MutexDestroy(mutex);
		ConditionDestroy(cond);
	}

	TEST(ThreadAtomics)
	{
		Atomic atomic = 17;
		CHECK( atomic == 17 );

		AtomicIncrement(&atomic);
		CHECK( atomic == 18 );

		AtomicDecrement(&atomic);
		AtomicDecrement(&atomic);
		CHECK( atomic == 16 );

		AtomicAdd(&atomic, 10);
		CHECK( atomic == 26 );

		AtomicWrite(&atomic, 33);
		CHECK( AtomicRead(&atomic) == 33 );
	}
}
