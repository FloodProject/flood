//////////////////////////////////////////////////////////////////////////
//
// Author:  Corbin Hart
// Project: Genesis
// Date:    1/6/2011
//
// Task - The basic classes necessary to create / monitor a task and its life time.
//
// Copyright 2010-2011 (All rights reserved)
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/Concurrency.h"

// -- Forward Declarations --
NAMESPACE_CORE_BEGIN

	// <summary>
	// This class represents a completion flag. It is a simple wrapper around an
	//	atomic integer.
	// </summary>
	class ALIGN_BEGIN(16) API_CORE Completion
	{
	private:
		volatile Atomic Status_;

	public:
		Completion() : Status_(0) {}

		INLINE bool Done() { return (AtomicRead(&Status_)) == 0; }
		INLINE void Set(bool isBusy)
		{
			if(isBusy)
				AtomicIncrement(&Status_);
			else
				AtomicDecrement(&Status_);
		}
	} ALIGN_END(16);

	// <summary>
	// Class wrapping the interface to actually do work. Contains functions to allow for more complicated behavior, such as
	//	stealing and spreading.
	// </summary>
	
	
	class ALIGN_BEGIN(16) API_CORE TaskBase
	{
	public:
		typedef std::vector<TaskBase *> Range;

	public:
		Completion * Completion_;

	public:
		TaskBase( Completion * comp )
			: Completion_(comp)
		{}
		virtual ~TaskBase() {}	// NOTE: DOES NOT DELETE COMPLETION_

		// <summary>
		// Where the actual work is done. Fill this out.
		// </summary>
		virtual void Do() = 0;

		// <summary>
		// Allocates new tasks and sub-divides the work based on data-parallelism, using a provided target granularity.
		// </summary>
		INLINE virtual Range * Split( uint32 numSlices ) { return nullptr; }

		// <summary>
		// Sub-divides the work, if possible, with a granularity of 1. This is intended to keep as much of the work as
		//	possible in the Worker thread's task queue, providing an opportunity for other worker threads to further steal
		//	from the work.
		// </summary>
		INLINE virtual bool  Slice( TaskBase *& outTask ) { return false; }
	}	ALIGN_END(16);

NAMESPACE_CORE_END