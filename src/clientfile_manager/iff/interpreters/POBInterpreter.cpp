#include "POBInterpreter.h"

#include "../iff.h"
#include "../filenode.h"

using namespace ClientFileManager::IFF::Interpreters;

void POBInterpreter::interpret(ClientFileManager::IFF::FileNode* node)
{
	if(node->name() == "0003DATA")
	{
	}
	else if(node->name() == "PRTL" || node->name() == "PRTSPRTL")
	{
	}
	else if(node->name() == "0005DATA")
	{
	}
	else if(node->name() == "0000VERT")
	{
	}
	else if(node->name() == "INDX")
	{
	}
	else if(node->name() == "PRTL0004")
	{
	}
	else if(node->name() == "LGHT")
	{
	}
	else if(node->name() == "0001META")
	{
	}
	else if(node->name() == "PNOD")
	{
	}
	else if(node->name() == "PEDG")
	{
	}
	else if(node->name() == "ECNT")
	{
	}
	else if(node->name() == "ESTR")
	{
	}
	else if(node->name() == "CRC ")
	{
	}
}