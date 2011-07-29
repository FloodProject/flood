#include <cstdio>

#define INSTANTIATE_TEMPLATES
#include "Core/API.h"
#include "Core/Reflection.h"

using namespace vapor;

struct MessageField
{
	
};

struct MessageBlock
{
	String name;
	String parent;
	std::vector<Field> fields;
};

struct ServiceBlock
{

};

std::vector<MessageBlock> messages;

struct Block
{
	String name;
	MessageBlock message;
	Primitive* primitive;
	Field field;
	FieldId fieldId;
	Class klass;
	ClassId classId;
};