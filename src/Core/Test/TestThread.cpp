/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Concurrency.h"
#include "Core/Task.h"
#include "Core/Memory.h"
#include <UnitTest++.h>

namespace {

int value = 20;

void Run(Thread*, void* data)
{
	int* num = (int*) data;
	value = *num;
}
int taskVal= 10;
void RunTask(Task* task)
{
	taskVal = 20;
}

bool added = false; 
void TaskEventWatch(TaskEvent taskEvent)
{
	switch(taskVal)
	{
		case 10:
			if(!added)
			{
				CHECK(taskEvent.state == TaskState::Added);
				added = true;
			}
			else
			{
				CHECK(taskEvent.state == TaskState::Started);
			}
			break;
		case 20:
			CHECK(taskEvent.state == TaskState::Finished);
			break;
	}
}

}

SUITE(Core)
{
	TEST(ThreadBasics)
	{
		Thread thread;

		ThreadFunction fn;
		fn.Bind(::Run);

		int data = 42;
		thread.start(fn, &data);
		thread.join();

		CHECK(42 == value);
	}

	TEST(ThreadConditions)
	{
		value = 20;

		Thread thread;

		Condition cond;
		Mutex mutex; 

		ThreadFunction fn;
		fn.Bind([=] (Thread* thread, void* data) {
			value = 30;
			auto cond = (Condition*)data;
			cond->wakeOne();
		});

		thread.start(fn, &cond);
		CHECK(20 == value);

		mutex.lock();

		// Prevents deadlock.
		while(value == 20)
		{
			cond.wait(mutex);
		}

		mutex.unlock();

		thread.join();
		CHECK( 30 == value);
	}

	TEST(ThreadAtomics)
	{
		Atomic<uint32> atomic(17);
		CHECK( atomic.read() == 17 );

		atomic.increment();
		CHECK( atomic.read() == 18 );

		atomic.decrement();
		atomic.decrement();
		CHECK( atomic.read() == 16 );

		atomic.add(10);
		CHECK( atomic.read() == 26 );
		
		atomic.write(33);
		CHECK( atomic.read() == 33 );
	}

	TEST(TaskpoolEvents)
	{
		TaskPool pool(1); 
		Task task;
		task.callback.Bind(RunTask);
		CHECK( taskVal == 10 );
		pool.onTaskEvent.Connect(TaskEventWatch);
		pool.add(&task, 1);
		pool.waitAll();
		CHECK( taskVal == 20 );

	}
}
