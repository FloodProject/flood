/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "CuTest.h"

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Serialization.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Stream.h"
#include "ReflectionTypes.h"

//-----------------------------------//

typedef Serializer* (*SerializerCreateFunction)(Allocator*);

void TestSerialization(CuTest* tc, SerializerCreateFunction SerializerCreate)
{
	Allocator* alloc = AllocatorGetHeap();
	Serializer* serializer = SerializerCreate(alloc);
	serializer->alloc = alloc;

	B instanceB;
	instanceB.change();
	
	SerializerSaveObjectToFile(serializer, "TestB.json", &instanceB);

	B* loadB = (B*) SerializerLoadObjectFromFile(serializer, "TestB.json");

	CuAssertIntEquals(tc, instanceB.bar, loadB->bar);
	CuAssertDblEquals(tc, instanceB.vec.z, loadB->vec.z, 0.01);
	CuAssertCppStrEquals(tc, instanceB.str, loadB->str);

	Deallocate(loadB);

	C instanceC;
	instanceC.allocate();
	instanceC.change();

	SerializerSaveObjectToFile(serializer, "TestC.json", &instanceC);

	C* loadC = (C*) SerializerLoadObjectFromFile(serializer, "TestC.json");
	Deallocate(loadC);

	Deallocate(serializer);
}

void TestSerializerJSON(CuTest* tc)
{
	TestSerialization(tc, SerializerCreateJSON);
}

void TestSerializerBinary(CuTest* tc)
{
	TestSerialization(tc, SerializerCreateBinary);
}

//-----------------------------------//

CuSuite* GetSuiteSerialization()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSerializerJSON);
	//SUITE_ADD_TEST(suite, TestSerializerBinary);
	return suite;
}