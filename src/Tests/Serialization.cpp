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

#if 0
	B instanceB;
	instanceB.change();
	
	SerializerSaveObjectToFile(serializer, StringFormat("TestB.%s", ext), &instanceB);

	B* loadB = (B*) SerializerLoadObjectFromFile(serializer, StringFormat("TestB.%s", ext));

	CuAssertIntEquals(tc, instanceB.george, loadB->george);
	CuAssertIntEquals(tc, instanceB.bar, loadB->bar);
	CuAssertDblEquals(tc, instanceB.vec.z, loadB->vec.z, 0.01);
	CuAssertCppStrEquals(tc, instanceB.str, loadB->str);

	Deallocate(loadB);

	C instanceC;
	instanceC.allocate();
	instanceC.change();

	SerializerSaveObjectToFile(serializer, StringFormat("TestC.%s", ext), &instanceC);

	C* loadC = (C*) SerializerLoadObjectFromFile(serializer, StringFormat("TestC.%s", ext));

	CuAssertIntEquals(tc, instanceC.anA->foo, loadC->anA->foo);
	CuAssertIntEquals(tc, instanceC.arrayA.size(), loadC->arrayA.size());

	for(size_t i = 0; i < instanceC.arrayA.size(); i++)
		CuAssertIntEquals(tc, instanceC.arrayA[i]->foo, loadC->arrayA[i]->foo);

	Deallocate(loadC);

	D instanceD;
	instanceD.allocate();
	SerializerSaveObjectToFile(serializer, StringFormat("TestD.%s", ext), &instanceD);
	
	D* loadD = (D*) SerializerLoadObjectFromFile(serializer, StringFormat("TestD.%s", ext));
	Deallocate(loadD);
#endif

	F instanceF;
	instanceF.allocate();
	SerializerSaveObjectToFile(serializer, StringFormat("TestF.%s", ext), &instanceF);
	
	F* loadF = (F*) SerializerLoadObjectFromFile(serializer, StringFormat("TestF.%s", ext));
	CuAssertIntEquals(tc, instanceF.vecA[0].foo, loadF->vecA[0].foo);
	CuAssertIntEquals(tc, instanceF.vecA[1].foo, loadF->vecA[1].foo);
	Deallocate(loadF);

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
	//SUITE_ADD_TEST(suite, TestSerializerJSON);
	SUITE_ADD_TEST(suite, TestSerializerBinary);
	return suite;
}