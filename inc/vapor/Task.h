/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ReferenceCount.h"

namespace vapor {

//-----------------------------------//

class CORE_API Task : public ReferenceCounted
{
	DECLARE_UNCOPYABLE(Task);

public:

	Task();
	virtual ~Task();

	// Runs the task.
	virtual void run() = 0;

	// Flags the task to finish.
	void flagFinish();

protected:

	// Keeps track if the task is to finish.
	bool finish;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Task );

//-----------------------------------//

} // end namespace
	