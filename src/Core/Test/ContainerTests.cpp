/************************************************************************
*
* Flood Project � (2008-201x)
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
        CHECK_EQUAL(0, v.size());
        
        v.push_back(3);

        CHECK_EQUAL(1, v.size());
        CHECK_EQUAL(3, v[0]);
    }

    TEST_FIXTURE(ContainerFixture, ArrayCopyCotrIndexOperator)
    {
        // recreate first test
        Array<int> v(*_a);
        v.push_back(3);

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
        v.push_back(3);

        CHECK(v.end() == v.begin() + v.size());
        CHECK_EQUAL(3, *v.begin());

        v.pop_back();
        CHECK(v.empty());
    }

    TEST_FIXTURE(ContainerFixture, ArrayInsertFront)
    {
        Array<int> v(*_a);
        Array<int> w(*_a);

        for(int i = 0; i < 3; ++i)
        {
            v.push_back(i);
            w.push_back(i + 10);
        }
        // v : 0,1,2
        // w : 10,11,12

        for(int i = 0; i < 3; ++i)
            v.push_back(i + 3);
        //v : 0,1,2,3,4,5
        
        v.insert(v.begin(), w.begin(), w.end());
        // v: 10,11,12,0,1,2,3,4,5
        CHECK(v[0] == 10 && v[1] == 11 && v[2] == 12 && v[3] == 0 && v[4] == 1 && v[5] == 2 && v[6] == 3 && v[7] == 4 && v[8] == 5);
    }

    TEST_FIXTURE(ContainerFixture, ArrayInsertEnd)
    {
        Array<int> v(*_a);
        Array<int> w(*_a);

        for(int i = 0; i < 3; ++i)
        {
            v.push_back(i);
            w.push_back(i + 10);
        }
        // v : 0,1,2
        // w : 10,11,12

        for(int i = 0; i < 3; ++i)
            v.push_back(i + 3);
        //v : 0,1,2,3,4,5

        v.insert(v.end(), w.begin(), w.end());
        // v: 0,1,2,3,4,5,10,11,12
        CHECK(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 3 && v[4] == 4 && v[5] == 5 && v[6] == 10 && v[7] == 11 && v[8] == 12);
    }

    TEST_FIXTURE(ContainerFixture, ArrayInsertMiddle)
    {
        Array<int> v(*_a);
        Array<int> w(*_a);

        for(int i = 0; i < 3; ++i)
        {
            v.push_back(i);
            w.push_back(i + 10);
        }
        // v : 0,1,2
        // w : 10,11,12

        for(int i = 0; i < 3; ++i)
            v.push_back(i + 3);
        //v : 0,1,2,3,4,5

        v.insert(&v[3], w.begin(), w.end());
        // v : 0,1,2,10,11,12,3,4,5
        CHECK(v[0] == 0 && v[1] == 1 && v[2] == 2 && v[3] == 10 && v[4] == 11 && v[5] == 12 && v[6] == 3 && v[7] == 4 && v[8] == 5);
    }

    TEST_FIXTURE(ContainerFixture, ArrayRemoveFront)
    {
        Array<int> v(*_a);

        for (int i=0; i<100; ++i)
            v.push_back(i);
        CHECK_EQUAL(100, v.size());

        v.remove(&v.front());
        
        CHECK_EQUAL(99, v.size());
        CHECK_EQUAL(1, v.front());
        CHECK_EQUAL(99, v.back());
    }

    TEST_FIXTURE(ContainerFixture, ArrayRemoveBack)
    {
        Array<int> v(*_a);

        for (int i=0; i<100; ++i)
            v.push_back(i);
        CHECK_EQUAL(100, v.size());

        v.remove(&v.back());

        CHECK_EQUAL(99, v.size());
        CHECK_EQUAL(0, v.front());
        CHECK_EQUAL(98, v.back());
    }

    TEST_FIXTURE(ContainerFixture, ArrayRemoveMiddle)
    {
        Array<int> v(*_a);

        for (int i=0; i<100; ++i)
            v.push_back(i);

        auto it = std::find(v.begin(), v.end(), 51);
        CHECK_EQUAL(51, *it);

        v.remove(it);
        CHECK_EQUAL(99, v.size());
        CHECK_EQUAL(0, v.front());
        CHECK_EQUAL(99, v.back());
        
        it = std::find(v.begin(), v.end(), 51);
        CHECK(it == v.end());
    }
}