/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "OIFFInterpreter.h"

#include "../../iff.h"
#include "../../filenode.h"
#include "../../../filemanager.h"


using namespace ClientFileManager::IFF::Interpreters;
using namespace std;
using namespace std::tr1::placeholders;

AttributeHandlerIndex OIFFInterpreter::attributeHandler_;

OIFFInterpreter::OIFFInterpreter()
	: IFFInterpreter(), _hasAggregate(false)
{
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("animationMapFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("appearanceFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("arrangementDescriptorFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clearFloraRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clientDataFile"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("cockpitFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionBlockFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionPassFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionHeight"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionLength"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialBlockFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialPassFlags"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerType"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerVolumeLimit"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("detailedDescription"), bind(&OIFFInterpreter::_handleClientString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("gameObjectType"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("hasWings"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("locationReservationRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("lookAtText"), bind(&OIFFInterpreter::_handleClientString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("movementDatatable"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("niche"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("noBuildRadius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("numberOfPoles"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("objectName"), bind(&OIFFInterpreter::_handleClientString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("onlyVisibleInTools"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("playerControlled"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("portalLayoutFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("race"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("radius"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scale"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scaleThresholdBeforeExtentTest"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("sendToClient"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slopeModAngle"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slopeModPercent"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slotDescriptorFilename"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("snapToTerrain"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("species"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("stepHeight"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("structureFootprintFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("swimHeight"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("targetable"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("terrainModificationFileName"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("tintPalette"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("useStructureFootprintOutline"), bind(&OIFFInterpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("warpTolerance"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("waterModPercent"), bind(&OIFFInterpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffect"), bind(&OIFFInterpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffectIndex"), bind(&OIFFInterpreter::_handleInt, _1, _2, _3)));

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
	const std::string& nameRef = node->name();
	if(nameRef == "XXXX")
	{
		_handleXXXX(node->data());
	}
	else if(nameRef == "DERVXXXX")
	{
		_handleDERVXXXX(node->data());
	}

}

void OIFFInterpreter::_handleXXXX(anh::ByteBuffer& buf)
{
	if(buf.size() > 0)
	{
		std::string attributeName = readStringFromByteBuffer(buf);
		AttributeHandlerIndexIterator it = attributeHandler_.find(attributeName);

		if(it != attributeHandler_.cend())
		{
			it->second(this, attributeName, buf);
		}
	}
}

void OIFFInterpreter::_handleDERVXXXX(anh::ByteBuffer& buf)
{
	parentFiles.insert(readStringFromByteBuffer(buf));
}

void OIFFInterpreter::_handlePCNT(anh::ByteBuffer& buf)
{
}

void OIFFInterpreter::_handleClientString(OIFFInterpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		shared_ptr<ClientString> cs = make_shared<ClientString>();
		if(buf.read<char>())
		{
			cs->file = readStringFromByteBuffer(buf);
			if(buf.read<char>())
			{
				cs->entry = readStringFromByteBuffer(buf);
			}
		}
		dst->attributes_.insert(make_pair<string, shared_ptr<ClientString>>(move(name), cs));
	}
}

void OIFFInterpreter::_handleString(OIFFInterpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->attributes_.insert(make_pair<string, shared_ptr<string>>(move(name), make_shared<string>(readStringFromByteBuffer(buf))));
	}
}

void OIFFInterpreter::_handleInt(OIFFInterpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		dst->attributes_.insert(make_pair<string, uint32_t>(move(name), buf.read<uint32_t>()));
	}
}

void OIFFInterpreter::_handleFloat(OIFFInterpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		dst->attributes_.insert(make_pair<string, float>(move(name), buf.read<float>()));
	}
}

void OIFFInterpreter::_handleBool(OIFFInterpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->attributes_.insert(make_pair<string, bool>(move(name), (buf.read<char>()) ? true : false));
	}
}

void OIFFInterpreter::debug()
{
	std::set<std::string>::iterator parentIt = parentFiles.begin();
	std::set<std::string>::iterator parentEnd = parentFiles.end();
	while(parentIt != parentEnd)
	{
		printf("Has Parent: %s\n", parentIt->c_str());
		++parentIt;
	}

	std::map<std::string, boost::any>::const_iterator it = attributes_.cbegin();

	while(it != attributes_.cend())
	{
		printf("Attribute: \"%s\", value=", it->first.c_str(), it->second.type().name());
		if(it->second.type() == typeid(std::uint32_t))
		{
			printf("%d", boost::any_cast<std::uint32_t>(it->second));
		}
		else if(it->second.type() == typeid(shared_ptr<std::string>))
		{
			printf("%s", boost::any_cast<shared_ptr<std::string>>(it->second)->c_str());
		}
		else if(it->second.type() == typeid(shared_ptr<ClientString>))
		{
			shared_ptr<ClientString> cs = boost::any_cast<shared_ptr<ClientString>>(it->second);
			printf("@%s:%s", cs->file.c_str(), cs->entry.c_str());
		}
		else if(it->second.type() == typeid(float))
		{
			printf("%f", boost::any_cast<float>(it->second));
		}
		else if(it->second.type() == typeid(shared_ptr<bool>))
		{
			printf("%s", (boost::any_cast<bool>(it->second)) ? "true" : "false");
		}
		printf("\n");
		++it;
	}
}

void OIFFInterpreter::loadAggregateData(ClientFileManager::FileManager& f)
{
	if(!_hasAggregate)
	{
		std::map<std::string, boost::any> aggregateAttributeMap;

		std::set<std::string>::iterator it = parentFiles.begin();
		std::set<std::string>::iterator end = parentFiles.end();
		while(it != end)
		{
			std::shared_ptr<OIFFInterpreter> subI = f.load<OIFFInterpreter>(*it);
			subI->loadAggregateData(f);

			//Now we continue to build up our map.
			std::map<std::string, boost::any>::iterator sub_it = subI->attributes_.begin();
			std::map<std::string, boost::any>::iterator sub_end = subI->attributes_.end();

			while(sub_it != sub_end)
			{
				aggregateAttributeMap.insert(*sub_it);
				++sub_it;
			}

			++it;
		}

		std::map<std::string, boost::any>::iterator old_it = attributes_.begin();
		std::map<std::string, boost::any>::iterator old_end = attributes_.end();
		while(old_it != old_end)
		{
			aggregateAttributeMap.insert(std::move(*old_it));
			++old_it;
		}

		attributes_ = std::move(aggregateAttributeMap);
		_hasAggregate = true;
	}
}