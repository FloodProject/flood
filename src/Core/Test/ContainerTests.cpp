/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Containers/Array.h"

#include <algorithm>

#include "UnitTest++.h"

using namespace fld;

SUITE(CoreTests_Containers)
{
    struct ContainerFixture
    {
        ContainerFixture()
            : _a(nullptr)
        {
            _a = AllocatorGetHeap();
        }

        ~ContainerFixture()
        {
            _a = nullptr;
        }

        ::Allocator * _a;
    };

    TEST_FIXTURE(ContainerFixture, ArraySizePushBack)
    {
        Array<int> v(*_a);
        CHECK_EQUAL(0, array::size(v));
        
        array::push_back(v, 3);

        CHECK_EQUAL(1, array::size(v));
        CHECK_EQUAL(3, v[0]);
    }

    TEST_FIXTURE(ContainerFixture, ArrayCopyCotrIndexOperator)
    {
        // recreate first test
        Array<int> v(*_a);
        array::push_back(v, 3);

        // copy constructor
        Array<int> v2(v);
        CHECK_EQUAL(3, v2[0]);

        v2[0] = 5;
        CHECK_EQUAL(3, v[0]);
        CHECK_EQUAL(5, v2[0]);

        v2 = v;
        CHECK_EQUAL(3, v2[0]);
    }

    TEST_FIXTURE(ContainerFixture, ArrayBeginEndPopBackEmpty)
    {
        Array<int> v(*_a);
        array::push_back(v, 3);

        CHECK(array::end(v) == array::begin(v) + array::size(v));
        CHECK_EQUAL(3, *array::begin(v));

        array::pop_back(v);
        CHECK(array::empty(v));
    }

    TEST_FIXTURE(ContainerFixture, ArrayInsertFront)
    {
        Array<int> v(*_a);
        Array<int> w(*_a);

        for(int i = 0; i < 3; ++i)
        {
            array::push_back(v, i);
            array::push_back(w, i + 10);
        }
        // v : 0,1,2
        // w : 10,11,12

        for(int i = 0; i < 3; ++i)
            array::push_back(v, i + 3);
        //v : 0,1,2,3,4,5
        
        array::insert(v, array::begin(v), array::begin(w), array::end(w));
        // v: 10,11,12,0,1,2,3,4,5
        CHECK(v[0] == 10 && v[1] == 11 && v[2] == 12 && v[3] == 0 && v[4] == 1 && v[5] == 2 && v[6] == 3 && v[7] == 4 && v[8] == 5);
    }

    TEST_FIXTURE(ContainerFixture, ArrayInsertEnd)
    {
        Array<int> v(*_a);
        Array<int> w(*_a);

        for(int i = 0; i < 3; ++i)
        {
            array::push_back(v, i);
            array::push_back(w, i + 10);
        }
        // v : 0,1,2
        // w : 10,11,12

        for(int i = 0; i < 3; ++i)
            array::push_back(v, i + 3);
        //v : 0,1,2,3,4,5

        array::insert(v, array::end(v), array::begin(w), array::end(w));
        // v: 0,1,2,3,4,5,10,11,12
        CHECK(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3 && v[4] == 4 && v[5] == 5 && v[6] == 10 && v[7] == 11 && v[8] == 12);
    }

    TEST_FIXTURE(ContainerFixture, ArrayInsertMiddle)
    {
        Array<int> v(*_a);
        Array<int> w(*_a);

        for(int i = 0; i < 3; ++i)
        {
            array::push_back(v, i);
            array::push_back(w, i + 10);
        }
        // v : 0,1,2
        // w : 10,11,12

        for(int i = 0; i < 3; ++i)
            array::push_back(v, i + 3);
        //v : 0,1,2,3,4,5

        array::insert(v, &v[3], array::begin(w), array::end(w));
        // v : 0,1,2,10,11,12,3,4,5
        CHECK(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 10 && v[4] == 11 && v[5] == 12 && v[6] == 3 && v[7] == 4 && v[8] == 5);
    }

    TEST_FIXTURE(ContainerFixture, ArrayRemoveFront)
    {
        Array<int> v(*_a);

        for (int i=0; i<100; ++i)
            array::push_back(v, i);
        CHECK_EQUAL(100, array::size(v));

        array::remove(v, &array::front(v));
        
        CHECK_EQUAL(99, array::size(v));
        CHECK_EQUAL(1, array::front(v));
        CHECK_EQUAL(99, array::back(v));
    }

    TEST_FIXTURE(ContainerFixture, ArrayRemoveBack)
    {
        Array<int> v(*_a);

        for (int i=0; i<100; ++i)
            array::push_back(v, i);
        CHECK_EQUAL(100, array::size(v));

        array::remove(v, &array::back(v));

        CHECK_EQUAL(99, array::size(v));
        CHECK_EQUAL(0, array::front(v));
        CHECK_EQUAL(98, array::back(v));
    }

    TEST_FIXTURE(ContainerFixture, ArrayRemoveMiddle)
    {
        Array<int> v(*_a);

        for (int i=0; i<100; ++i)
            array::push_back(v, i);

        auto it = std::find(array::begin(v), array::end(v), 51);
        CHECK_EQUAL(51, *it);

        array::remove(v, it);
        CHECK_EQUAL(99, array::size(v));
        CHECK_EQUAL(0, array::front(v));
        CHECK_EQUAL(99, array::back(v));
        
        it = std::find(array::begin(v), array::end(v), 51);
        CHECK(it == array::end(v));
    }
}