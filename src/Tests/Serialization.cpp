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

void TestSerialization(CuTest* tc, SerializerCreateFunction SerializerCreate, const char* ext)
{
	Allocator* alloc = AllocatorGetHeap();
	Serializer* serializer = SerializerCreate(alloc);
	serializer->alloc = alloc;

	B instanceB;
	instanceB.change();
	
	SerializerSaveObjectToFile(serializer, StringFormat("TestB.%s", ext), &instanceB);

	B* loadB = (B*) SerializerLoadObjectFromFile(serializer, StringFormat("TestB.%s", ext));

	CuAssertIntEquals(tc, instanceB.bar, loadB->bar);
	CuAssertDblEquals(tc, instanceB.vec.z, loadB->vec.z, 0.01);
	CuAssertCppStrEquals(tc, instanceB.str, loadB->str);

	Deallocate(loadB);

	C instanceC;
	instanceC.allocate();
	instanceC.change();

	SerializerSaveObjectToFile(serializer, StringFormat("TestC.%s", ext), &instanceC);

	C* loadC = (C*) SerializerLoadObjectFromFile(serializer, StringFormat("TestC.%s", ext));
	Deallocate(loadC);

	Deallocate(serializer);
}

void TestSerializerJSON(CuTest* tc)
{
	TestSerialization(tc, SerializerCreateJSON, "json");
}

void TestSerializerBinary(CuTest* tc)
{
	TestSerialization(tc, SerializerCreateBinary, "bin");
}

//-----------------------------------//

CuSuite* GetSuiteSerialization()
{
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSerializerJSON);
	SUITE_ADD_TEST(suite, TestSerializerBinary);
	return suite;
}