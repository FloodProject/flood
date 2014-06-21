/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Handle.h"
#include "Core/References.h"

#include <UnitTest++.h>

namespace
{
    struct A : public ReferenceCounted
    {
    };

    struct B : public A
    {
    };

    class R : public ReferenceCounted
    {
    };

    int test = 10;

    HandleManager* HandleManagerA;
    ReferenceCounted* HandleFindA(HandleId id) { return HandleFind(HandleManagerA, id); }
    void  HandleDestroyA(HandleId id) { HandleDestroy(HandleManagerA, id); }

    typedef Handle<A, HandleFindA, HandleDestroyA> HandleA;
    typedef Handle<B, HandleFindA, HandleDestroyA> HandleB;

    HandleA GetTempHandleA(A* instance)
    {
        return HandleCreate(HandleManagerA, instance); 
    }
}

SUITE(Core)
{
    TEST(Handles)
    {
        HandleManagerPtr pHandleManagerA( pHandleCreateManager(AllocatorGetHeap()) );
        HandleManagerA = pHandleManagerA.get();

        CHECK_EQUAL( 0, HandleManagerA->nextHandle.read());

        A* instance = AllocateHeap(A);

        HandleA handle  = HandleCreate(HandleManagerA, instance);
        HandleA handle2 = handle;

        HandleB handleB = HandleCast<B>(handle);
        B* instanceB = handleB.Resolve();

#ifdef COMPILER_SUPPORTS_CXX11
        HandleB handleB2 = std::move(handleB);
        CHECK_EQUAL(HandleInvalid, handleB.getId());
        handleB = std::move(handleB2);
#endif

        CHECK(instance == instanceB);

        CHECK_EQUAL(1, HandleManagerA->nextHandle.read());
        CHECK_EQUAL(1, handle.getId());
        CHECK(instance == handle.Resolve());
        CHECK_EQUAL(3, ReferenceGetCount((instance)));

        // Explicit destroy of the handle.
        handle.Destroy();

        CHECK(nullptr == handle.Resolve());
        CHECK(nullptr == handle2.Resolve());

        // Test explicit set of handle ids.
        A* a3 = AllocateHeap(A);

        HandleA handle3 = HandleCreate(HandleManagerA, a3);
        HandleA handle4 = handle3;

        CHECK_EQUAL(2, a3->references.read());
        handle3.setId(HandleInvalid);
        CHECK_EQUAL(1, a3->references.read());
        handle4.setId(HandleInvalid);

        CHECK(nullptr == handle3.Resolve());

        CHECK_EQUAL(0, HandleManagerA->handles.Size());
    }

    TEST(RefCounts)
    {
        R* rp = AllocateHeap(R);

        RefPtr<R> rpx = rp;
        CHECK_EQUAL(1, ReferenceGetCount(rp));
        CHECK_EQUAL(rp, rpx.px);

        RefPtr<R> rpx2 = rpx;
        CHECK_EQUAL(2, ReferenceGetCount(rp));

        // Copy constructor.
        R* rp2 = AllocateHeap(R, *rp);
        CHECK_EQUAL(0, ReferenceGetCount(rp2));

        rpx2 = rp2;
        CHECK_EQUAL(rp2, rpx2.px);

        // Copy assignment operator.
        rp2 = rp;
        CHECK_EQUAL(ReferenceGetCount(rp), ReferenceGetCount(rp2));
    }
}