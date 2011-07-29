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
#include "Core/References.h"

namespace {

struct A : public ReferenceCounted
{
};

struct B : public A
{
};

int test = 10;

HandleManager* HandleManagerA;
void* HandleFindA(HandleId id) { return HandleFind(HandleManagerA, id); }
void  HandleDestroyA(HandleId id) { HandleDestroy(HandleManagerA, id); }

typedef Handle<A, HandleFindA, HandleDestroyA> HandleA;
typedef Handle<B, HandleFindA, HandleDestroyA> HandleB;

HandleA GetTempHandleA(A* instance)
{
	return HandleCreate(HandleManagerA, instance); 
}

void TestHandle(CuTest* tc)
{
	HandleManagerPtr pHandleManagerA( pHandleCreateManager(AllocatorGetHeap()) );
	HandleManagerA = pHandleManagerA.get();

	CuAssertIntEquals(tc, 0, AtomicRead(&HandleManagerA->nextHandle));

	A* instance = Allocate(A,  AllocatorGetHeap() );
	
	HandleA handle  = HandleCreate(HandleManagerA, instance);
	HandleA handle2 = handle;

	HandleB handleB = HandleCast<B>(handle);
	B* instanceB = handleB.Resolve();

#ifdef COMPILER_SUPPORTS_CPP0X
	HandleB handleB2 = std::move(handleB);
	CuAssertIntEquals(tc, HandleInvalid, handleB.id);
	handleB = std::move(handleB2);
#endif

	CuAssertPtrEquals(tc, instance, instanceB);
	
	CuAssertIntEquals(tc, 1, AtomicRead(&HandleManagerA->nextHandle));
	CuAssertIntEquals(tc, 1, handle.id);
	CuAssertPtrEquals(tc, instance, handle.Resolve());
	CuAssertIntEquals(tc, 3, ReferenceGetCount(instance));

	// Explicit destroy of the handle.
	handle.Destroy( AllocatorGetHeap() );

	CuAssertPtrEquals(tc, nullptr, handle.Resolve());
	CuAssertPtrEquals(tc, nullptr, handle2.Resolve());
	CuAssertIntEquals(tc, 0, HandleManagerA->handles.size());
}

class R : public ReferenceCounted
{
};

void TestRefCount(CuTest* tc)
{
	R* rp = Allocate(R,  AllocatorGetHeap());
	
	RefPtr<R> rpx = rp;
	CuAssertIntEquals(tc, 1, ReferenceGetCount(rp));
	CuAssertPtrEquals(tc, rp, rpx.px);

	RefPtr<R> rpx2 = rpx;
	CuAssertIntEquals(tc, 2, ReferenceGetCount(rp));

	// Copy constructor.
	R* rp2 = Allocate(R,  AllocatorGetHeap(), *rp);
	CuAssertIntEquals(tc, 0, ReferenceGetCount(rp2));

	rpx2 = rp2;
	CuAssertPtrEquals(tc, rp2, rpx2.px);

	// Copy assignment operator.
	rp2 = rp;
	CuAssertIntEquals(tc, ReferenceGetCount(rp), ReferenceGetCount(rp2));
}

}

CuSuite* GetSuiteHandle()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHandle);
	SUITE_ADD_TEST(suite, TestRefCount);
    return suite;
}