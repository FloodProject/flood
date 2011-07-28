#include <cstdio>
#include <string>
#include <vector>

enum BlockType
{
	BT_MESSAGE,
	BT_ENUM,
	BT_SERVICE
};

struct MessageField
{
};

struct EnumBlock
{
	
};

struct MessageBlock
{
	std::string name;
	std::string parent;
	bool inherits;
	std::vector<MessageField> fields;
};

struct ServiceBlock
{

};

std::vector<MessageBlock> messages;

MessageBlock message;

struct Block
{
	std::string name;
	std::string type;
};