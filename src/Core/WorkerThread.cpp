/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"

#include "Core/Task.h"
#include "Core/WorkerThread.h"
#include "Core/WorkerThreadPool.h"

#include "Core/Memory.h"
#include "Core/Containers/Array.h"
#include "Core/Log.h"

NAMESPACE_CORE_BEGIN

	// -- Thread-local storage for the current "this" pointer
	//	This can be used by tasks to access the thread they are running on
	namespace {
		thread_local WorkerThread * LocalWorkerThread_ = nullptr;
	}

#pragma region Static Accessor / Ctr & Dtr / Init & Shutdown

	// -- Accessor for the current thread --
	WorkerThread * WorkerThread::This()
	{
		return LocalWorkerThread_;
	}

	// -- Constructor / Destructor --
	WorkerThread::WorkerThread()
		: Index_( 0 )
		, Pool_(nullptr)
		, CurrentCompletion_(nullptr)
		, Tasks_(*AllocatorGetHeap())
	{}

	WorkerThread::~WorkerThread()
	{
		assert( Pool_ == nullptr );
	}

	// -- Init / Shutdown --
	void WorkerThread::Initialize(WorkerThreadPool * pool, uint32 threadIndex)
	{
		//BOOST_LOG( gn_log::get() ) << "(Concurrent) \tWorker[ " << threadIndex << " ] starting up...\n";
		LogInfo("WorkerThread[%i] starting up...", threadIndex);
		
		Index_ = threadIndex;
		Pool_ = pool;
		TaskMutex_.reset( MutexCreate(AllocatorGetHeap()) );
		Tasks_.reserve(TaskCapacity);	// some random number of tasks
		
		if(Index_ != 0)	// so long as we're not the main thread...
		{
			//Thread_ = boost::thread( boost::bind( &Worker::Run, this ) );	// create the boost::thread to run our shit
			ThreadFunction runFn = MakeDelegate(this, &WorkerThread::Run);

			Thread_.reset( ThreadCreate(AllocatorGetHeap()) );
			ThreadStart(Thread_, runFn, nullptr);
		}
		// TODO: Fix this.
		else	// Set this here, as the Run() function, which sets it otherwise, won't get called.
			LocalWorkerThread_ = this;
			//ThisThread_.reset( (const_cast<Worker *>(this)) );
	}

	void WorkerThread::Shutdown()
	{
		//BOOST_LOG( gn_log::get() ) << "(Concurrent) \tWorker[ " << Index_ << " ] joining...\n";
		LogInfo("WorkerThread[%i] joining...", Index_);
		
		if(Index_ != 0)
		{
			//Thread_.join();
			ThreadJoin(Thread_.get());
		}
		else
			LocalWorkerThread_ = nullptr;
			//ThisThread_.release();
		

		Tasks_.clear();
		TaskMutex_.reset();
		Pool_ = nullptr;
		Index_ = static_cast<uint32>(-1);
	}

#pragma endregion
#pragma region Run

	void WorkerThread::Run(Thread*, void*)
	{
		// set the thread-local-storage pointer to "this"
		LocalWorkerThread_ = this;
		//ThisThread_.reset( (const_cast<Worker *>(this)) ); // TODO: Fix this.

		// infinitely loop
		for(;;)
		{
			// go idle
			Idle();

			// check to see if the pool wants to exit
			//	- if so, break out of the loop
			if( Pool_->IsDone_ )
				break;

			// loop and check to see if the pool thinks there is work to be done
			while(Pool_->CurrentCompletion_ != nullptr)
			{
				// do work
				Work(nullptr);	// -> null is passed to designate that this is not a call from within a task

				// check to see if the pool wants to exit
				// - if so, break out of the loop
				if(Pool_->IsDone_)
					break;
			}
		}

		// release the thread-local-storage
		LocalWorkerThread_ = nullptr;
		//ThisThread_.release();	// TODO: Fix this.
	}
	
#pragma endregion
#pragma region Work / Idle / Steal / GiveTo

	void WorkerThread::Work( Completion * waitingFlag )
	{
		// begin stealing loop
		do {
			// set up temporaries
			TaskBase * task = nullptr;
			Completion * prevCompletion = nullptr;
			
			// loop on popping a task
			while( Pop(task) )
			{
				// store previous completion
				prevCompletion = CurrentCompletion_;

				// set the current completion to the task's completion
				CurrentCompletion_ = task->Completion_;

				// run the task
				task->Do();

				// set the current completion as done
				CurrentCompletion_->Set(false);

				// restore the old completion
				CurrentCompletion_ = prevCompletion;

				// if the taskFlag is not null (then someone is waiting on us) and taskFlag is done (another thread isn't still working on it)
					// return from working
				if(waitingFlag && waitingFlag->Done())
					return;
			}
		} while( Steal() ); // end stealing loop
	}

	void WorkerThread::Idle()
	{
		// decrease the active worker count
		AtomicDecrement(&Pool_->ActiveWorkers_);

		// were we the last active worker?
		if(AtomicRead(&Pool_->ActiveWorkers_) == 1)
			// notify the pool that everyone is now idle
			ConditionWakeAll(Pool_->AllWorkersIdle_);

		// wait until we get woken up by the pool, immediately unlock our locked local mutex
		MutexLock(TaskMutex_);
		ConditionWait(Pool_->WakeupNotifiction_, TaskMutex_);
		MutexUnlock(TaskMutex_);
		
		// increase the number of active workers
		AtomicIncrement(&Pool_->ActiveWorkers_);
	}

	bool WorkerThread::Steal()
	{
		// TODO: Traverse in psuedo-random pattern (see GCC3)
		// loop through all the threads
		for(auto w = Pool_->Workers_.begin(); w != Pool_->Workers_.end(); ++w)
		{
			// if we're looking at "this" thread, continue
			if( *w == this )
				continue;

			// if the thread we're looking at gave up some work
			//  return true
			if( (*w)->GiveTo(this) )
				return true;

			// if in the interim, tasks were pushed into the internal queue
			//  return true (because we got work)
			if( !Tasks_.empty() )
				return true;
		}

		// return false
		return false;
	}
	
	bool WorkerThread::GiveTo( WorkerThread * thief )
	{
		/*// try to lock this thread
		boost::mutex::scoped_try_lock this_lock( TaskMutex_ );

		// if we can't, return false
		if(!this_lock)
			return false;
		*/

		// acquire a lock on the current thread
		MutexLock(TaskMutex_);

		// if we have no tasks, return false
		if(Tasks_.empty())
		{
			MutexUnlock(TaskMutex_);
			return false;
		}

		// lock the thieving thread
		//boost::mutex::scoped_lock thief_lock( thief->TaskMutex_ );
		MutexLock(thief->TaskMutex_);

		// if the thieving thread got tasks in the interim, return false
		if(thief->Tasks_.size() > 0)
		{
			MutexUnlock(thief->TaskMutex_);
			MutexUnlock(TaskMutex_);
			return false;
		}

		// if there is only one task
		if(Tasks_.size() == 1)
		{
			// attempt to split it in half
			TaskBase * outTask = nullptr;
			if( Tasks_[0]->Slice( outTask ) )
			{
				outTask->Completion_->Set(true);
				thief->Tasks_.push_back(outTask);
			}
			else
			{
				thief->Tasks_.push_back(Tasks_[0]);
				Tasks_.pop_back();
			}
			
			MutexUnlock(thief->TaskMutex_);
			MutexUnlock(TaskMutex_);
			return true;
		}

		// calculate half of the current tasks this thread has
		size_t splitDepth = ( Tasks_.size() + 1 ) / 2;

		thief->Tasks_.resize(splitDepth );

		// for each task to be stolen
		for(size_t i = 0; i < splitDepth; ++i)
		{
			// copy each task into the thieving thread's task stack
			thief->Tasks_[i] = Tasks_[i];

			// in-place copy the i-th task with the i-th task beyond the split point
			Tasks_[i] = Tasks_[(i + splitDepth) % Tasks_.size()];
		}

		//memcpy( thief->Tasks_[0],	Tasks_[0],			(splitDepth * sizeof(TaskBase *))); // copy from me to them
		//memcpy( Tasks_[0],			Tasks_[splitDepth], (splitDepth * sizeof(TaskBase *))); // copy down

		// cut out the old tasks
		Tasks_.resize(Tasks_.size() - splitDepth);
		assert( Tasks_.capacity() == WorkerThread::TaskCapacity );	// make sure that the STL doesn't deallocate our memory

		MutexUnlock(thief->TaskMutex_);
		MutexUnlock(TaskMutex_);
		return true;
	}

#pragma endregion
#pragma region AssignTask / Pop

	bool WorkerThread::AssignTask( TaskBase * newTask )
	{
		// check if there is only one thread.. if so, just bail
		if(Pool_->WorkersCount() == 1)
			return false;

		// if there is no root task (threads are idle / finishing sub-tasks)
		if( Pool_->CurrentCompletion_ == nullptr )
		{
			// ASSERT that this is the main thread
			assert( WorkerThread::This()->Index_ == 0 );

			// enforce all workers idle
			Pool_->WaitUntilIdle();

			// attempt to split the task by the number of threads
			scoped_ptr<TaskBase::Range> slices(newTask->Split( Pool_->WorkersCount() ));
			if(slices != nullptr)
			{
				// assign each thread a task (INCLUDING THIS ONE)
				for(uint32 i = 0; i < slices->size(); ++i)
				{
					WorkerThread * w = Pool_->Workers_[i];
					//boost::mutex::scoped_lock worker_lock( w->TaskMutex_ );
					MutexLock(w->TaskMutex_);
					
					(*slices)[i]->Completion_->Set(true);
					w->Tasks_.push_back((*slices)[i] );
					
					MutexUnlock(w->TaskMutex_);
				}

				// set the root task in the pool to newTask
				Pool_->CurrentCompletion_ = newTask->Completion_;

				// wake all the threads
				Pool_->WakeAll();

				return true;// return the love
			}
		}

		// lock this thread's task vector
		{
			//boost::mutex::scoped_lock this_lock(TaskMutex_);
			MutexLock(TaskMutex_);

			// if the task vector is full, return false
			if( Tasks_.size() >= WorkerThread::TaskCapacity )
			{
				MutexUnlock(TaskMutex_);
				return false;
			}

			// set the task as busy
			newTask->Completion_->Set(true);

			// assign the job
			Tasks_.push_back(newTask );

			MutexUnlock(TaskMutex_);
		}

		// if there is no root task (threads are idle / finishing sub-tasks)
		if( Pool_->CurrentCompletion_ == nullptr )
		{
			// set the root task in the pool to newTask
			Pool_->CurrentCompletion_ = newTask->Completion_;

			// wake any idle threads
			Pool_->WakeAll();
		}

		return true; // return the love
	}

	bool WorkerThread::Pop( TaskBase *& outTask )
	{
		// lock this thread's task vector
		//boost::mutex::scoped_lock this_lock( TaskMutex_ );
		MutexLock(TaskMutex_);

		// if the task vector is empty, return false
		if( Tasks_.empty() )
		{
			MutexUnlock(TaskMutex_);
			return false;
		}

		// get a pointer to the last task in the vector
		TaskBase * t = Tasks_.back();

		// if the task can slice itself into a new task (e.g. several iterations of a loop)
		if( t->Slice( outTask ) )
		{
			// mark the task's completion flag as busy
			t->Completion_->Set( true );

			MutexUnlock(TaskMutex_);
			return true; // return the love
		}

		// assign the outTask to the last task
		outTask = t;

		// pop the task out of the vector
		Tasks_.pop_back();

		MutexUnlock(TaskMutex_);
		return true;// return the love
	}

#pragma endregion
#pragma region Push / WorkUntil

	bool WorkerThread::Push( TaskBase * newTask )
	{
		// if PushInternal works, return true as the work is now assigned
		if( AssignTask( newTask ) )
			return true;

		// otherwise, just run the task serially and return false
		newTask->Do();
		return false;
	}

	void WorkerThread::YieldUntil( Completion * flag )
	{
		// while the task that we're waiting on is not finished, do some work
		while( !flag->Done() )
			Work( flag );

		// if the flag is owned by the current root task, then since it's done the Pool should be notified
		// NOTE: Eventually other workers will see that there is no current main task to do, and after they are
		//	done chewing through their own internal shit they will go idle waiting for the main thread to assign
		//	them more work.
		if( Pool_->CurrentCompletion_ == flag )
			Pool_->CurrentCompletion_ = nullptr;
	}

#pragma endregion

NAMESPACE_CORE_END