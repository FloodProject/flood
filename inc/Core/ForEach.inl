//////////////////////////////////////////////////////////////////////////
//
// Author:  Corbin Hart
// Project: Genesis
// Date:    2/21/2011
//
// ForEach - Implementation for a parallel for-each.
//
// Copyright 2010-2011 (All rights reserved)
//////////////////////////////////////////////////////////////////////////

NAMESPACE_CORE_BEGIN

	template < class IterType >
	ForTask<IterType>::ForTask(Completion * comp, IterType begin, IterType end, Func const & func, uint32 minSliceSize )
		: TaskBase( comp )
		, Begin_(begin)
		, End_(end)
		, Func_(func)
		, MinSliceSize_(minSliceSize)
	{}

	template < class IterType >
	void ForTask<IterType>::Do()
	{
		Func_( Begin_, End_ );

		delete this; // I don't like doing this..
	}

	template < class IterType >
	TaskBase::Range * ForTask<IterType>::Split( uint32 numSlices )
	{
		// first, check to see if no split is yielded from division
		uint32 sliceSize = (End_ - Begin_);

		if(sliceSize < (MinSliceSize_ * 2) || numSlices == 1)
			return nullptr;
		
		// second, check to see if splitting yields a smaller than minimum sized range
		sliceSize /= numSlices;
		uint32 rollover = 0; // used to hold the difference of items if the sliceSize <= minSliceSize

		if( sliceSize < MinSliceSize_ )
		{
			numSlices = (End_ - Begin_) / MinSliceSize_;
			rollover = (End_ - Begin_) % MinSliceSize_;
			sliceSize = MinSliceSize_;
		}

		assert(numSlices > 1); // NOTE: if this fails, something is fucked up in my logic

		// allocate the vector / reserve the memory
		Range * tasks = new Range();
		tasks->reserve(numSlices);

		// push a pointer to "this" into the vector
		tasks->push_back(this);

		// re-set the end of "this" slice / set up temporary
		//	NOTE: "this" slice contains any rollover if there is an uneven spread of the range across target slices
		End_ = Begin_ + sliceSize + rollover;
		IterType currentBegin = End_;

		// set up each task
		//	- NOTE: we start at 1 because we've implicitly created one slice already, by modifying the
		//		current task's End_ variable
		for(uint32 i = 1; i < numSlices; ++i)
		{
			TaskBase * newTask = new ForTask( this->Completion_, currentBegin, currentBegin + sliceSize, Func_, MinSliceSize_ );
			tasks->push_back(newTask);
			currentBegin += sliceSize;
		}

		return tasks;
	}

	template < class IterType >
	bool ForTask<IterType>::Slice( TaskBase *& outTask )
	{
		uint32 sliceSize = (End_ - Begin_) / 2;
		if( sliceSize < MinSliceSize_ )
			return false;

		outTask = new ForTask(this->Completion_, Begin_ + sliceSize, End_, Func_, MinSliceSize_);
		End_ = Begin_ + sliceSize;

		return true;
	}

NAMESPACE_CORE_END