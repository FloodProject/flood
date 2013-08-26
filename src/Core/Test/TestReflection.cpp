/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Core/API.h"
#include "Core/Memory.h"
#include "Core/Reflection.h"
#include "Core/Object.h"
#include "Core/Serialization.h"
#include "Core/ClassWatcher.h"
#include "ReflectionTypes.h"

#include <UnitTest++.h>

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
		CHECK_EQUAL(enumE, ReflectionDatabase::GetDatabase().findType("E"));

		CHECK_EQUAL("E", enumE->name);
		CHECK_EQUAL(sizeof(E), enumE->size);
		CHECK_EQUAL((uint8)TypeKind::Enumeration, (uint8)enumE->kind);
		CHECK_EQUAL(E::Max, enumE->values.size());

		CHECK_EQUAL("F1", enumE->getValueName(E::F1));
		CHECK_EQUAL(E::F1, enumE->getValue("F1"));
	}

	TEST(ClassA)
	{
		Class* klassA = AGetType();
		CHECK(nullptr != klassA);
		CHECK_EQUAL(klassA, ReflectionDatabase::GetDatabase().findType("A"));

		CHECK(klassA->isComposite());

		A bar;
		A* foo = AllocateHeap(A);

		CHECK_EQUAL(20, bar.foo);
		CHECK_EQUAL(20, foo->foo);

		Deallocate(foo);

		A* instanceA = (A*) klassA->createInstance(AllocatorGetHeap());

		Field* field = klassA->getField("foo");
		CHECK(nullptr != field);
		CHECK(field->type->isPrimitive());

		FieldSet<int32>(field, instanceA, 10);
		CHECK_EQUAL(10, instanceA->foo);

		Deallocate(instanceA);
	}

	TEST(ClassB)
	{
		Class* klassB = BGetType();
		CHECK(nullptr != klassB);
		CHECK_EQUAL(klassB, ReflectionDatabase::GetDatabase().findType("B"));

		CHECK(klassB->inherits(AGetType()));
		CHECK(!AGetType()->inherits(klassB));
	}

	TEST(Objects)
	{
		ReflectionDatabase& typedb = ReflectionDatabase::GetDatabase();

		for(auto it : typedb.types)
		{
			Type* type = it.second;
			if (!type->isComposite()) continue;

			Class* klass = (Class*) type;

			if (!klass->inherits(ObjectGetType()))
				continue;

			if (klass->isAbstract()) continue;

			Object* object = (Object*) klass->createInstance(AllocatorGetHeap());

			Class* runtimeClass = Class::GetType(object);
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

	namespace
	{
		static uint8 ident = 0;
		#define indentSpaces() for(auto i = 0; i < ident; i++) { printf(" "); }

		static void WalkComposite(ReflectionContext* c, ReflectionWalkType wt)
		{
			if(wt == ReflectionWalkType::End)
			{
				ident--;
				return;
			}

			printf("%s\n", c->composite->name);
			ident++;
		}

		static void WalkCompositeField(ReflectionContext* c, ReflectionWalkType wt)
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
		context.walkCompositeField = WalkCompositeField;
		context.walkEnum = nullptr;
		context.walkPrimitive = nullptr;

		ReflectionHandleContextMap handleContextMap;
		context.handleContextMap = &handleContextMap;

		A instanceA;
		B instanceB;

		ReflectionWalk(&instanceA, &context);
		ReflectionWalk(&instanceB, &context);
	}
}