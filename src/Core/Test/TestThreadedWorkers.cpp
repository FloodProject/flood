/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Log.h"
#include "Core/Utilities.h"

#include "Core/WorkerThreadPool.h"
#include "Core/WorkerThread.h"
#include "Core/Task.h"
#include "Core/ForEach.h"

#include <array>
#include <functional>

#include <UnitTest++.h>

namespace {

	//////////////////////////////////////////////////////////////////////////
	// - Simple task that sleeps for a period of time before logging its completion
	class ALIGN_BEGIN(16) SimpleWaitTask : public TaskBase
	{
	public:
		SimpleWaitTask(Completion * comp)
			: TaskBase(comp)
		{}

		INLINE virtual void Do()
		{
			LogInfo("SimpleTask::Do - Running on thread[%i]", WorkerThread::This()->Index());
			LogInfo("SimpleTask::Do - Sleeping for 200ms...");
			SystemSleep(200);
			LogInfo("SimpleTask::Do - Done.");
		}
	} ALIGN_END(16);

	//////////////////////////////////////////////////////////////////////////
	// - Simple Task that increments a per-thread-counter to observe how tasks
	//		are distributed throughout the WorkerThread pool
	class ALIGN_BEGIN(16) SimpleTask : public TaskBase
	{
	public:
		//typedef std::array<volatile Atomic, 4> AtomicVector;
		typedef std::array<uint32, 4> OutputVector;

	private:
		OutputVector * Counters_;

	public:
		SimpleTask(Completion * comp, OutputVector * counters)
			: TaskBase(comp)
			, Counters_(counters)
		{}

		INLINE virtual void Do()
		{
			if(Counters_ == nullptr)
				return;

			(*Counters_)[ WorkerThread::This()->Index() ] += 1;
		}
	} ALIGN_END(16);

	//////////////////////////////////////////////////////////////////////////
	// - Task that linearly adds a range of numbers together and supports slicing of
	//		the target range in half, in order to share / distribute the workload
	//		across the pool of WorkerThreads.
	class ALIGN_BEGIN(16) SliceableTask : public TaskBase
	{
	public:
		typedef std::array<uint32, 4> OutputVector;
		typedef uint32 InputIter;

		static volatile Atomic SliceCount;
		static OutputVector TaskCountPerThread;

	private:
		OutputVector * Output_;
		InputIter Head_;
		InputIter Tail_;

	public:
		SliceableTask(Completion * cmp, OutputVector * output, InputIter head, InputIter tail)
			: TaskBase(cmp)
			, Output_(output)
			, Head_(head)
			, Tail_(tail)
		{}

		INLINE virtual void Do()
		{
			for(auto i = Head_; i < Tail_; ++i)
				(*Output_)[WorkerThread::This()->Index()] += i;

			TaskCountPerThread[ WorkerThread::This()->Index() ] += 1;

			// HACK : I really don't like doing this..
			delete this;
			//SliceableTask ** _this = &(const_cast<SliceableTask *>(this)); 
			//(*_this) = nullptr;
		}

		INLINE virtual bool Slice(TaskBase *& outTask)
		{
			// determine if the slice is less than our minimum slice size
			uint32 sliceSize = (Tail_ - Head_) / 2;
			if(sliceSize < 4)
				return false;

			// allocate a new SliceableTask on the heap, with our new information
			outTask = new SliceableTask(this->Completion_, Output_, Head_ + sliceSize, Tail_);
			Tail_ = Head_ + sliceSize;

			// increment the number of slices
			AtomicIncrement(&SliceCount);

			return true;
		}

	} ALIGN_END(16);

	volatile Atomic SliceableTask::SliceCount = 0;
	SliceableTask::OutputVector SliceableTask::TaskCountPerThread = SliceableTask::OutputVector();

	typedef scoped_ptr<SimpleTask> SimpleTaskPtr;

	//////////////////////////////////////////////////////////////////////////
	// - Simple wrapper around the ForTask that does some debug information gathering
	//		such as the number of times it was split, number of times sliced, and
	//		how many individual segments of the range were executed per-thread.
	template < typename IterType >
	class ALIGN_BEGIN(16) ProfiledForTask : public ForTask<IterType>
	{
	public:
		typedef std::function< void (IterType, IterType) > Func;
		typedef std::array<uint32, 4> TaskCountOutput;

	private:
		TaskCountOutput * TaskCounter_;
		volatile Atomic * SplitCount_;
		volatile Atomic * SliceCount_;


	public:
		ProfiledForTask(Completion * comp, IterType begin, IterType end, Func const & func, IterType minSliceSize
			, TaskCountOutput * taskCounter, volatile Atomic * splitCount, volatile Atomic * sliceCount)
			: ForTask(comp, begin, end, func, minSliceSize)
			, TaskCounter_(taskCounter)
			, SplitCount_(splitCount)
			, SliceCount_(sliceCount)
		{}

		// - Has the same behavior as the ForTask, excepting that it counts
		INLINE virtual void Do()
		{
			Func_(Begin_, End_);

			// annotate the task iteration
			(*TaskCounter_)[WorkerThread::This()->Index()] += 1;

			// HACK : I HATE THIS I DON'T LIKE IT MAKE IT STOP
			delete this;
		}

		// - Forwards to the derived Split
		virtual TaskBase::Range * Split( uint32 numSlices )
		{
			TaskBase::Range * splits = ForTask::Split(numSlices);

			if(splits != nullptr)
				AtomicAdd(SplitCount_, splits->size());

			return splits;
		}

		INLINE virtual bool Slice( TaskBase *& outTask )
		{
			uint32 sliceSize = (End_ - Begin_) / 2;
			if( sliceSize < MinSliceSize_ )
				return false;

			outTask = new ProfiledForTask(this->Completion_, Begin_ + sliceSize, End_, Func_, MinSliceSize_,
							TaskCounter_, SplitCount_, SliceCount_);
			End_ = Begin_ + sliceSize;

			// annotate the slice increment
			AtomicIncrement(SliceCount_);

			return true;
		}

	} ALIGN_END(16);

	template < class IterType >
	void ProfiledForEach( IterType begin, IterType end, typename ProfiledForTask<IterType>::Func func, uint32 minSliceSize = 16)
	{
		LogInfo("ProfiledForEach test starting... Range size: %i", (end - begin));

		typedef typename ProfiledForTask<IterType>::TaskCountOutput TaskCounter;
		
		TaskCounter counters; //{0,0,0,0};
		for(uint32 i = 0; i < counters.size(); ++i)
			counters[i] = 0;

		volatile Atomic splits = 0;
		volatile Atomic slices = 0;

		Completion completion;
		ProfiledForTask<IterType> * task = new ProfiledForTask<IterType>( &completion, begin, end, func, minSliceSize,
			&counters, &splits, &slices);	// this will get deleted whenever the Do finishes
		WorkerThread::This()->Push( task );
		WorkerThread::This()->YieldUntil( &completion );

		LogInfo("ProfiledForEach done...");
		LogInfo("  Total Splits: %i ...", splits);
		LogInfo("  Total Slices: %i ...", slices);

		uint32 count = 0;
		for(uint32 i = 0; i < counters.size(); ++i)
		{
			LogInfo("  WorkerThread[%i]: %i executions.. "
				, i
				,  counters[i]);

			count += counters[i];
		}

		LogInfo("  Total tasks executed: %i", count);
	}
}

SUITE(Core)
{
	TEST(WorkerThreadBasics)
	{
		// create the pool
		scoped_ptr<WorkerThreadPool> pool(new WorkerThreadPool());
		pool->Make( 4 );

		// verify the number of worker threads
		CHECK( 4 == pool->WorkersCount() );

		// verify thread 0 == WorkerThread::This()
		auto workers = pool->WorkersBeginEnd();
		CHECK_EQUAL(WorkerThread::This(), *workers.first);

		// push a test task
		Completion cf;
		SimpleWaitTask * st = new SimpleWaitTask(&cf);
		
		CHECK( WorkerThread::This()->Push(st) );

		WorkerThread::This()->YieldUntil(&cf);

		// verify the wake logic is correct for the thread pool
		CHECK_EQUAL(4, pool->ActiveWorkersCount());

		// verify the idle logic is correct for the thread pool
		pool->WaitUntilIdle();
		CHECK_EQUAL(1, pool->ActiveWorkersCount());	// NOTE: the main thread should always be counted as active

		delete st;
	}

	TEST(WorkerThreadTaskBasics)
	{
		// create the pool
		scoped_ptr<WorkerThreadPool> pool(new WorkerThreadPool());
		pool->Make( 4 );

		// verify the number of worker threads
		CHECK( 4 == pool->WorkersCount() );

		// create a basic non-splittable task
		SimpleTask::OutputVector counters = {0,0,0,0};

		Completion stComp;
		std::vector<SimpleTask *> simpleTasks;
		for(uint32 i = 0; i < 10000; ++i)
			simpleTasks.push_back( new SimpleTask(&stComp, &counters) );

		LogInfo("SimpleTask test starting...");

		for(uint32 i = 0; i < simpleTasks.size(); ++i)
			WorkerThread::This()->Push( simpleTasks[i] );

		WorkerThread::This()->YieldUntil(&stComp);

		// verify the task is run and the thread the task is on
		LogInfo("SimpleTask test done...");
		uint32 count = 0;
		for(uint32 i = 0; i < counters.size(); ++i)
		{
			LogInfo("  WorkerThread[%i]: %i executions..", i, counters[i]);
			count += counters[i];
		}

		LogInfo("  Total tasks executed: %i", count);

		CHECK(count == simpleTasks.size());

		for(uint32 i = 0; i < simpleTasks.size(); ++i)
			delete simpleTasks[i];

		simpleTasks.swap( std::vector<SimpleTask *>() );

		// create a simple sliceable task to be shared amongst the whole thread pool
		for(uint32 i = 0; i < counters.size(); ++i)
			counters[i] = 0;

		Completion slComp;
		static const uint32 maxTail = 1000000;
		SliceableTask * sliceTask = new SliceableTask(&slComp, &counters, 0, maxTail);

		LogInfo("SliceableTask test starting...");

		CHECK( WorkerThread::This()->Push(sliceTask) );
		WorkerThread::This()->YieldUntil(&slComp);

		// verify the task has been split across all the threads
		LogInfo("SliceableTask test done...");
		LogInfo("  Total Slices: %i ...", SliceableTask::SliceCount);

		count = 0;
		for(uint32 i = 0; i < counters.size(); ++i)
		{
			LogInfo("  WorkerThread[%i]: %i executions.. Partial Sum: %u"
				, i
				, SliceableTask::TaskCountPerThread[i]
				, counters[i]);

			count += SliceableTask::TaskCountPerThread[i];
		}

		LogInfo("  Total tasks executed: %i", count);

		CHECK(count == (SliceableTask::SliceCount + 1));

		// -- FOR EACH
		
		// create a simple ForEach task
		std::array<uint32, 4> sums = {0,0,0,0};

		ProfiledForEach<uint32>(0, 100000, [&](uint32 begin, uint32 end) -> void {
			uint32 count = 0;
			for(auto i = begin; i < end; ++i)
				count += i;
			sums[WorkerThread::This()->Index()] += count;
		}, 16);

		uint64 totalSum = 0;
		uint32 w = 0;
		for(auto i = sums.begin(); i != sums.end(); ++i)
		{
			LogInfo("   WorkerThread[%i]: Partial Sum: %u", w++, *i);
			totalSum += *i;
		}

		LogInfo("   Total sum: %u", totalSum);

		// verify that the ForEach task is split across all the threads and run
	}

	TEST(WorkerThreadMetaStress)
	{
		// create the thread pool

		// ...
	}
}