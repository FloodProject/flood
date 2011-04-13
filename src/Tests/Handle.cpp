/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Handle.h"

using namespace vapor;

struct A : public ReferenceCounted
{
};

HandleManager* HandleManagerA;
void* HandleFindA(HandleId id) { return HandleFind(HandleManagerA, id); }
void  HandleDestroyA(HandleId id) { HandleDestroy(HandleManagerA, id); }

typedef Handle<A, HandleFindA, HandleDestroyA> HandleA;

void TestHandle(CuTest* tc)
{
	HandleManagerPtr pHandleManagerA( pHandleCreateManager(AllocatorGetHeap()) );
	HandleManagerA = pHandleManagerA.get();

	CuAssertIntEquals(tc, 0, AtomicRead(&HandleManagerA->nextHandle));

	A* instance = Allocate<A>( AllocatorGetHeap() );
	
	HandleA handle  = HandleCreate(HandleManagerA, instance);
	HandleA handle2 = handle;
	
	CuAssertIntEquals(tc, 1, AtomicRead(&HandleManagerA->nextHandle));
	CuAssertIntEquals(tc, 1, handle.id);
	CuAssertPtrEquals(tc, instance, handle.Resolve());
	CuAssertIntEquals(tc, 2, instance->getReferenceCount());

	// Explicit destroy of the handle.
	handle.Destroy( AllocatorGetHeap() );

	CuAssertPtrEquals(tc, nullptr, handle.Resolve());
	CuAssertPtrEquals(tc, nullptr, handle2.Resolve());
	CuAssertIntEquals(tc, 0, HandleManagerA->handles.size());
}

CuSuite* GetSuiteHandle() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHandle);
    return suite;
}