/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Memory.h"

static int value = 20;

static void Run(Thread*, void* data)
{
	int* num = (int*) data;
	value = *num;
}

void TestThreads(CuTest* tc)
{
	ThreadPtr thread( pThreadCreate(AllocatorGetHeap()) );

	ThreadFunction fn;
	fn.Bind(Run);

	int data = 42;
	ThreadStart(thread, fn, &data);
	ThreadJoin(thread);

	CuAssertIntEquals(tc, 42, value);
}

static Condition* cond;
static Mutex* mutex;

static void Run2(Thread*, void* data)
{
	value = 30;
	ConditionWakeOne(cond);
}

void TestCondition(CuTest* tc)
{
	value = 20;

	ThreadPtr thread( pThreadCreate(AllocatorGetHeap()) );

	cond = ConditionCreate( AllocatorGetHeap() );
	mutex = MutexCreate( AllocatorGetHeap() );
	
	ThreadFunction fn;
	fn.Bind(Run2);

	ThreadStart(thread, fn, nullptr);
	CuAssertIntEquals(tc, 20, value);
	
	MutexLock(mutex);

	// Prevents deadlock.
	while(value == 20)
	{
		ConditionWait(cond, mutex);
	}

	MutexUnlock(mutex);

	ThreadJoin(thread);
	CuAssertIntEquals(tc, 30, value);

	MutexDestroy(mutex);
	ConditionDestroy(cond);
}

void TestAtomics(CuTest* tc)
{	
	Atomic atomic = 17;
	CuAssertIntEquals(tc, 17, atomic);

	AtomicIncrement(&atomic);
	CuAssertIntEquals(tc, 18, atomic);

	AtomicDecrement(&atomic);
	AtomicDecrement(&atomic);
	CuAssertIntEquals(tc, 16, atomic);

	AtomicAdd(&atomic, 10);
	CuAssertIntEquals(tc, 26, atomic);

	AtomicWrite(&atomic, 33);
	CuAssertIntEquals(tc, 33, AtomicRead(&atomic));
}

CuSuite* GetSuiteConcurrency() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestThreads);
	SUITE_ADD_TEST(suite, TestCondition);
	SUITE_ADD_TEST(suite, TestAtomics);
    return suite;
}