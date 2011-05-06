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
#include "Core/Reflection.h"
#include "Core/Object.h"

using namespace vapor;

namespace {

//-----------------------------------//

enum_class E
{
	F1 = 0, F2, F3, Max
};

REFLECT_ENUM(E)
	ENUM(F1)
	ENUM(F2)
	ENUM(F3)
REFLECT_ENUM_END()

void TestEnum(CuTest* tc)
{
	Enum* enumE = EGetType();
	CuAssertPtrNotNull(tc, enumE);
	CuAssertPtrEquals(tc, enumE, ReflectionFindType("E"));

	CuAssertStrEquals(tc, "E", enumE->name);
	CuAssertIntEquals(tc, sizeof(E), enumE->size);
	CuAssertIntEquals(tc, Type::Enumeration, enumE->type);
	CuAssertIntEquals(tc, E::Max, enumE->values.size());

	CuAssertStrEquals(tc, "F1", EnumGetValueName(enumE, E::F1));
	CuAssertIntEquals(tc, E::F1, EnumGetValue(enumE, "F1"));
}

//-----------------------------------//

struct A : public Object
{
	A() : foo(20), foos(E::F1) { }

	int32 foo;
	E foos;
};

REFLECT_CLASS(A)
	FIELD_PRIMITIVE(int32, foo)
	FIELD_ENUM(E, foos)
REFLECT_CLASS_END()

void TestClassA(CuTest* tc)
{
	Class* klassA = ReflectionGetType(A);
	CuAssertPtrNotNull(tc, klassA);
	CuAssertPtrEquals(tc, klassA, ReflectionFindType("A"));

	CuAssertTrue(tc, ReflectionIsComposite(klassA));

	A bar;
	A* foo = Allocate(A, AllocatorGetHeap());

	CuAssertIntEquals(tc, 20, bar.foo);
	CuAssertIntEquals(tc, 20, foo->foo);

	Deallocate<A>(foo);

	A* instanceA = (A*) ClassCreateInstance( klassA, AllocatorGetHeap() );

	//const Class& klassA2 = instanceA->getType();
	//CuAssertPtrEquals(tc, &klassA, (Class*) &klassA2);

	Field* field = ClassGetField(klassA, "foo");
	CuAssertPtrNotNull(tc, field);
	CuAssertTrue(tc, ReflectionIsPrimitive(field->type));

	FieldSet<int32>(field, instanceA, 10);
	CuAssertIntEquals(tc, 10, instanceA->foo);

	Deallocate<A>(instanceA);
}

//-----------------------------------//

struct B : public A
{
};

REFLECT_CHILD_CLASS(B, A)
REFLECT_CLASS_END()

void TestClassB(CuTest* tc)
{
	Class* klassB = ReflectionGetType(B);
	CuAssertPtrNotNull(tc, klassB);
	CuAssertPtrEquals(tc, klassB, ReflectionFindType("B"));

	CuAssertTrue(tc, ClassInherits(klassB, AGetType()));
	CuAssertTrue(tc, !ClassInherits(AGetType(), klassB));
}

//-----------------------------------//

void TestObjects(CuTest* tc)
{
	ReflectionTypeMap& types = ReflectionGetTypeMap();

	for( auto it = types.begin(); it != types.end(); it++ )
	{
		Type* type = it->second;
		if( !ReflectionIsComposite(type) ) continue;

		Class* klass = (Class*) type;
		
		if( !ClassInherits(klass, ReflectionGetType(Object)) )
			continue;

		if( ClassIsAbstract(klass) ) continue;

		Object* object = (Object*) ClassCreateInstance(klass, AllocatorGetHeap());

		Class* runtimeClass = object->getType();
		CuAssertPtrEquals(tc, klass, runtimeClass);

		Deallocate(object);
	}
}

NAMESPACE_END

//-----------------------------------//

CuSuite* GetSuiteReflection()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestClassA);
	SUITE_ADD_TEST(suite, TestClassB);
	SUITE_ADD_TEST(suite, TestObjects);
    return suite;
}