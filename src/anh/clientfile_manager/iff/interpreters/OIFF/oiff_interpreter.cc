/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "oiff_interpreter.h"

#include "../../iff.h"
#include "../../filenode.h"
#include "../../../filemanager.h"


using namespace anh::clientfile_manager::iff::interpreters;
using namespace std;
using namespace std::tr1::placeholders;

AttributeHandlerIndex oiff_interpreter::attributeHandler_;

oiff_interpreter::oiff_interpreter()
	: interpreter_interface(), _hasAggregate(false)
{
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("animationMapFilename"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("appearanceFilename"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("arrangementDescriptorFilename"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clearFloraRadius"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clientDataFile"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("cockpitFileName"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionBlockFlags"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionFlags"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionPassFlags"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionHeight"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionLength"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialBlockFlags"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialFlags"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialPassFlags"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionRadius"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerType"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerVolumeLimit"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("detailedDescription"), bind(&oiff_interpreter::_handleClientString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("gameObjectType"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("hasWings"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("locationReservationRadius"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("lookAtText"), bind(&oiff_interpreter::_handleClientString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("movementDatatable"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("niche"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("noBuildRadius"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("numberOfPoles"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("objectName"), bind(&oiff_interpreter::_handleClientString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("onlyVisibleInTools"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("playerControlled"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("portalLayoutFilename"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("race"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("radius"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scale"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scaleThresholdBeforeExtentTest"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("sendToClient"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slopeModAngle"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slopeModPercent"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slotDescriptorFilename"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("snapToTerrain"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("species"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("stepHeight"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("structureFootprintFileName"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("swimHeight"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("targetable"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("terrainModificationFileName"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("tintPalette"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("useStructureFootprintOutline"), bind(&oiff_interpreter::_handleBool, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("warpTolerance"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("waterModPercent"), bind(&oiff_interpreter::_handleFloat, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffect"), bind(&oiff_interpreter::_handleString, _1, _2, _3)));
	attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffectIndex"), bind(&oiff_interpreter::_handleInt, _1, _2, _3)));

	//attackType
	//certificationsRequired
	//customizationVariableMapping
	//paletteColorCustomizationVariables
	//rangedIntCustomizationVariables
	//constStringCustomizationVariables
	//socketDestinations
	//surfaceType

}

void oiff_interpreter::interpret(std::shared_ptr<anh::clientfile_manager::iff::file_node> node)
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

void oiff_interpreter::_handleXXXX(anh::ByteBuffer& buf)
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

void oiff_interpreter::_handleDERVXXXX(anh::ByteBuffer& buf)
{
	parentFiles.insert(readStringFromByteBuffer(buf));
}

void oiff_interpreter::_handlePCNT(anh::ByteBuffer& buf)
{
}

void oiff_interpreter::_handleClientString(oiff_interpreter* dst, string& name, anh::ByteBuffer& buf)
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

void oiff_interpreter::_handleString(oiff_interpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->attributes_.insert(make_pair<string, shared_ptr<string>>(move(name), make_shared<string>(readStringFromByteBuffer(buf))));
	}
}

void oiff_interpreter::_handleInt(oiff_interpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		dst->attributes_.insert(make_pair<string, uint32_t>(move(name), buf.read<uint32_t>()));
	}
}

void oiff_interpreter::_handleFloat(oiff_interpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		dst->attributes_.insert(make_pair<string, float>(move(name), buf.read<float>()));
	}
}

void oiff_interpreter::_handleBool(oiff_interpreter* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->attributes_.insert(make_pair<string, bool>(move(name), (buf.read<char>()) ? true : false));
	}
}

void oiff_interpreter::debug()
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

void oiff_interpreter::loadAggregateData(anh::clientfile_manager::file_manager& f)
{
	if(!_hasAggregate)
	{
		std::map<std::string, boost::any> aggregateAttributeMap;

		std::set<std::string>::iterator it = parentFiles.begin();
		std::set<std::string>::iterator end = parentFiles.end();
		while(it != end)
		{
			std::shared_ptr<oiff_interpreter> subI = f.load<oiff_interpreter>((*it));
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