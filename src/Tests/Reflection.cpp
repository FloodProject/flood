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
#include "Core/Serialization.h"
#include "Core/ClassWatcher.h"
#include "ReflectionTypes.h"

REFLECT_ENUM(E)
	ENUM(F1)
	ENUM(F2)
	ENUM(F3)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(A, Object)
	FIELD_PRIMITIVE(0, int32, foo)
	FIELD_ENUM(1, E, foos)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(B, A)
	FIELD_PRIMITIVE(4, bool, george)
	FIELD_PRIMITIVE(5, uint32, bar)
	FIELD_PRIMITIVE(6, Vector3, vec)
	FIELD_PRIMITIVE(7, Quaternion, quat)
	FIELD_PRIMITIVE(8, Color, color)
	FIELD_PRIMITIVE(9, string, str)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(C, Object)
	FIELD_CLASS_PTR(11, A, A*, anA, RawPointer)
	FIELD_VECTOR_PTR(12, A, A*, arrayA, RawPointer)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(D, Object)
	FIELD_CLASS_PTR(5, Object, Object*, object, RawPointer)
	FIELD_CLASS_PTR(6, A, RefPtr<A>, refA, RefPointer)
	FIELD_VECTOR_PTR(7, A, RefPtr<A>, vecA, RefPointer)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(F, Object)
	FIELD_VECTOR(7, A, vecA)
	FIELD_CLASS(8, A, a)
REFLECT_CLASS_END()

//-----------------------------------//

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

	Deallocate(foo);

	A* instanceA = (A*) ClassCreateInstance( klassA, AllocatorGetHeap() );

	Field* field = ClassGetField(klassA, "foo");
	CuAssertPtrNotNull(tc, field);
	CuAssertTrue(tc, ReflectionIsPrimitive(field->type));

	FieldSet<int32>(field, instanceA, 10);
	CuAssertIntEquals(tc, 10, instanceA->foo);

	Deallocate(instanceA);
}

//-----------------------------------//

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

		Class* runtimeClass = ClassGetType(object);
		CuAssertPtrEquals(tc, klass, runtimeClass);

		Deallocate(object);
	}
}

//-----------------------------------//

void TestClassWatcher(CuTest* tc)
{
	B instanceB;

	ClassWatch* watch = ClassWatchCreate( AllocatorGetHeap() );
	ClassWatchAddFields(watch, &instanceB);

	FieldWatchVector watches;
	ClassWatchUpdate(watch, watches);

	CuAssertIntEquals(tc, 0, watches.size());

	instanceB.change();

	ClassWatchUpdate(watch, watches);
	CuAssertIntEquals(tc, 4, watches.size());

	watches.clear();
	ClassWatchUpdate(watch, watches);
	CuAssertIntEquals(tc, 0, watches.size());

	Deallocate(watch);
}

//-----------------------------------//

static uint8 ident = 0;
#define indentSpaces() for(auto i = 0; i < ident; i++) { printf(" "); }

static void WalkComposite(ReflectionContext* c, ReflectionWalkType::Enum wt)
{
	if(wt == ReflectionWalkType::End)
	{
		ident--;
		return;
	}

	printf("%s\n", c->composite->name);
	ident++;
}

static void walkCompositeField(ReflectionContext* c, ReflectionWalkType::Enum wt)
{
	if(wt != ReflectionWalkType::Begin) return;

	indentSpaces();
	printf("%s %s\n", c->field->type->name, c->field->name);
}

void TestWalker(CuTest* tc)
{
	ReflectionContext context;
	context.walkComposite = WalkComposite;
	context.walkCompositeField = walkCompositeField;
	context.walkEnum = nullptr;
	context.walkPrimitive = nullptr;

	A instanceA;
	B instanceB;
	
	ReflectionWalk(&instanceA, &context);
	ReflectionWalk(&instanceB, &context);
}

//-----------------------------------//

CuSuite* GetSuiteReflection()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestClassA);
	SUITE_ADD_TEST(suite, TestClassB);
	SUITE_ADD_TEST(suite, TestObjects);
	SUITE_ADD_TEST(suite, TestClassWatcher);
	//SUITE_ADD_TEST(suite, TestWalker);
    return suite;
}