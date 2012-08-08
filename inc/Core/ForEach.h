//////////////////////////////////////////////////////////////////////////
//
// Author:  Corbin Hart
// Project: Genesis
// Date:    2/21/2011
//
// ForEach - The base wrapper class around a concurrent for-each.
//
// Copyright 2010-2011 (All rights reserved)
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/API.h"
#include "Core/Task.h"

#include <functional>

NAMESPACE_CORE_BEGIN

	template < class IterType >
	class ALIGN_BEGIN(16) ForTask : public TaskBase
	{
		// -- Types --
	public:
		typedef std::function< void (IterType, IterType) > Func;

		// -- Member Data --
	protected:
		IterType Begin_;
		IterType End_;
		Func Func_;			// the function to call each iteration, passing in the current iterator
		uint32 MinSliceSize_;	// the smallest size that a slice of the range should be; example: do not sub-divide a range 16 items long

	public:
		ForTask(Completion * comp, IterType begin, IterType end, Func const & func, uint32 minSliceSize );

		// <summary>
		// Iterates between the beginning and the end, calling the provided function object passing the current iterator. Once
		//	completed, "this" is deleted.
		// </summary>
		INLINE virtual void Do();

		// <summary>
		// Attempts to split the range evenly across numSlices. There are two conditions that this may not happen:
		//	1) (End_ - Begin_) < (MinSliceSize_ * 2) : In this instance, Split will simply return nullptr, and not attempt
		//		to sub-divide the task.
		//	2) ((End_ - Begin_) / numSlices) < MinSliceSize_ : Here, numSlices becomes the smallest number to hold completely MinSliceSize_
		//		as a range. Any rollover is placed into the "this" task.
		// </summary>
		virtual TaskBase::Range * Split( uint32 numSlices );

		// <summary>
		// Attempts to slice the range in half. This can fail if (End_ - Begin_) < (MinSliceSize_ * 2), in which case outTask is left
		//	alone.
		// </summary>
		INLINE virtual bool Slice( TaskBase *& outTask );
	}	ALIGN_END(16);

	// <summary>
	// Provides a convenient function-call syntax to iterate across a range in parallel.
	//	NOTE: I don't like just putting 16 down for the minSliceSize.. but what can you do?
	// </summary>
	template < class IterType >
	void ForEach( IterType begin, IterType end, typename ForTask<IterType>::Func const & func, uint32 minSliceSize = 16)
	{
		Completion completion;
		ForTask<IterType> * task = new ForTask<IterType>( &completion, begin, end, func, minSliceSize );	// this will get deleted whenever the Do finishes
		WorkerThread::This()->Push( task );
		WorkerThread::This()->YieldUntil( &completion );
	}

NAMESPACE_CORE_END

#include "ForEach.inl"