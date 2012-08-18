//////////////////////////////////////////////////////////////////////////
//
// Flood © (2008-2013)
// CoreReflection - Conversion of the Reflection tests in vapor3d.
//
//////////////////////////////////////////////////////////////////////////

#include "Core/API.h"
#include "UnitTest++.h"

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

SUITE(Core)
{
	TEST(Enums)
	{
		Enum* enumE = EGetType();
		CHECK(enumE);
		CHECK_EQUAL(enumE, ReflectionFindType("E"));

		CHECK_EQUAL("E", enumE->name);
		CHECK_EQUAL(sizeof(E), enumE->size);
		CHECK_EQUAL((uint8)Type::Enumeration, (uint8)enumE->type);
		CHECK_EQUAL(E::Max, enumE->values.size());

		CHECK_EQUAL("F1", EnumGetValueName(enumE, E::F1));
		CHECK_EQUAL(E::F1, EnumGetValue(enumE, "F1"));
	}

	TEST(ClassA)
	{
		Class* klassA = ReflectionGetType(A);
		CHECK(nullptr != klassA);
		CHECK_EQUAL(klassA, ReflectionFindType("A"));

		CHECK(ReflectionIsComposite(klassA));

		A bar;
		A* foo = AllocateHeap(A);

		CHECK_EQUAL(20, bar.foo);
		CHECK_EQUAL(20, foo->foo);

		Deallocate(foo);

		A* instanceA = (A*) ClassCreateInstance( klassA, AllocatorGetHeap() );

		Field* field = ClassGetField(klassA, "foo");
		CHECK(nullptr != field);
		CHECK(ReflectionIsPrimitive(field->type));

		FieldSet<int32>(field, instanceA, 10);
		CHECK_EQUAL(10, instanceA->foo);

		Deallocate(instanceA);
	}

	TEST(ClassB)
	{
		Class* klassB = ReflectionGetType(B);
		CHECK(nullptr != klassB);
		CHECK_EQUAL(klassB, ReflectionFindType("B"));

		CHECK(ClassInherits(klassB, AGetType()));
		CHECK(!ClassInherits(AGetType(), klassB));
	}

	TEST(Objects)
	{
		ReflectionDatabase& typedb = ReflectionGetDatabase();

		for( auto it = typedb.types.begin(); it != typedb.types.end(); it++ )
		{
			Type* type = it->second;
			if( !ReflectionIsComposite(type) ) continue;

			Class* klass = (Class*) type;

			if( !ClassInherits(klass, ReflectionGetType(Object)) )
				continue;

			if( ClassIsAbstract(klass) ) continue;

			Object* object = (Object*) ClassCreateInstance(klass, AllocatorGetHeap());

			Class* runtimeClass = ClassGetType(object);
			CHECK_EQUAL(klass, runtimeClass);

			Deallocate(object);
		}
	}

	TEST(ClassWatcher)
	{
		B instanceB;

		ClassWatch* watch = ClassWatchCreate( AllocatorGetHeap() );
		ClassWatchAddFields(watch, &instanceB);

		FieldWatchVector watches;
		ClassWatchUpdate(watch, watches);

		CHECK_EQUAL(0, watches.size());

		instanceB.change();

		ClassWatchUpdate(watch, watches);
		CHECK_EQUAL(4, watches.size());

		watches.clear();
		ClassWatchUpdate(watch, watches);
		CHECK_EQUAL(0, watches.size());

		Deallocate(watch);
	}

	namespace {
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
	}

	TEST(Walker)
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
}