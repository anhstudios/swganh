/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "OIFFInterpreter.h"

#include "../../iff.h"
#include "../../filenode.h"

using namespace ClientFileManager::IFF::Interpreters;
using namespace std;
using namespace std::tr1::placeholders;

AttributeHandlerIndex OIFFInterpreter::attributeHandler_;

OIFFInterpreter::OIFFInterpreter()
	: IFFInterpreter()
{
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("DERVXXXX"), bind(&OIFFInterpreter::_handleDERVXXXX, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("XXXX"), bind(&OIFFInterpreter::_handleXXXX, this, _1)));

	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("objectName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("detailedDescription"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("lookAtText"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slotDescriptorFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("arrangementDescriptorFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("appearanceFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("portalLayoutFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clientDataFile"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("terrainModificationFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("structureFootprintFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("cockpitFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffect"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerType"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialPassFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialBlockFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionPassFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionBlockFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerVolumeLimit"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("gameObjectType"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("numberOfPoles"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffectIndex"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scale"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scaleThresholdBeforeExtentTest"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clearFloraRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("noBuildRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("locationReservationRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("radius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("snapToTerrain"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("tintPalette"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("sendToClient"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("onlyVisibleInTools"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("useStructureFootprintOutline"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("targetable"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("hasWings"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("playerControlled"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));

	//attackType
	//certificationsRequired
	//customizationVariableMapping
	//paletteColorCustomizationVariables
	//rangedIntCustomizationVariables
	//constStringCustomizationVariables
	//socketDestinations
	//surfaceType

}

void OIFFInterpreter::interpret(std::shared_ptr<ClientFileManager::IFF::FileNode> node)
{
	NodeNameIndexIterator it = nameLookup_.find(node->name());
	if( it != nameLookup_.cend())
	{
		it->second(node->data());
	}
}

void OIFFInterpreter::_handleXXXX(anh::ByteBuffer& buf)
{
	std::string attributeName = readStringFromByteBuffer(buf);
	AttributeHandlerIndexIterator it = attributeHandler_.find(attributeName);

	if(it != attributeHandler_.cend())
	{
		it->second(this, attributeName, buf);
	}
	else
	{
		printf("Unhandled Attribute: %s\n", attributeName.c_str());
	}

}

void OIFFInterpreter::_handleDERVXXXX(anh::ByteBuffer& buf)
{
	parentFiles.push_back(readStringFromByteBuffer(buf));
}

void OIFFInterpreter::_handlePCNT(anh::ByteBuffer& buf)
{
}

void OIFFInterpreter::_handleString(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf)
{
	char flag = buf.read<char>();
	if(flag)
	{
		dst->stringAttributes.insert(std::make_pair<std::string, std::string>(name, readStringFromByteBuffer(buf)));
	}
}

void OIFFInterpreter::_handleInt(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		dst->intAttributes.insert(std::make_pair<std::string, std::uint32_t>(name, buf.read<std::uint32_t>()));
	}
}

void OIFFInterpreter::_handleFloat(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		dst->intAttributes.insert(std::make_pair<std::string, float>(name, buf.read<float>()));
	}
}

void OIFFInterpreter::_handleBool(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->intAttributes.insert(std::make_pair<std::string, std::uint32_t>(name, (buf.read<char>()) ? true : false));
	}

}
