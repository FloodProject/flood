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

void TestSerializerJSON(CuTest* tc)
{
#if 0
	B instanceB;
	SaveObjectToFile("TestB.json", &instanceB);

	B* loadB = (B*) LoadObjectFromFile("TestB.json");
	Deallocate(loadB);
#endif

	C instanceC;
	instanceC.change();
	SerializerSaveObjectToFile("TestC.json", &instanceC);

	C* loadC = (C*) SerializerLoadObjectFromFile("TestC.json");
	Deallocate(loadC);
}

//-----------------------------------//

CuSuite* GetSuiteSerialization()
{
    CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestSerializerJSON);
    return suite;	
}