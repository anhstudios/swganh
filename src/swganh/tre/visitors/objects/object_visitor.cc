// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "object_visitor.h"

#include "../../iff/iff.h"
#include "../../iff/filenode.h"
#include "../../iff/foldernode.h"

#include <anh/resource/resource_manager_interface.h>

#include <swganh/tre/visitors/slots/slot_arrangement_visitor.h>
#include <swganh/tre/visitors/slots/slot_descriptor_visitor.h>

using namespace swganh::tre;
using namespace std;
using namespace std::tr1::placeholders;

AttributeHandlerIndex ObjectVisitor::attributeHandler_;

ObjectVisitor::ObjectVisitor()
	: VisitorInterface(), has_aggregate_(false)
{
	if(attributeHandler_.empty())
	{
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("animationMapFilename"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("appearanceFilename"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("arrangementDescriptorFilename"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clearFloraRadius"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("clientDataFile"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("cockpitFilename"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionBlockFlags"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionFlags"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionActionPassFlags"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionHeight"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionLength"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialBlockFlags"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialFlags"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionMaterialPassFlags"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionRadius"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerType"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("containerVolumeLimit"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("detailedDescription"), bind(&ObjectVisitor::_handleClientString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("gameObjectType"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("hasWings"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("interiorLayoutFileName"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("locationReservationRadius"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("lookAtText"), bind(&ObjectVisitor::_handleClientString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("movementDatatable"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("niche"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("noBuildRadius"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("numberOfPoles"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("objectName"), bind(&ObjectVisitor::_handleClientString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("onlyVisibleInTools"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("playerControlled"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("portalLayoutFilename"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("race"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("radius"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scale"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("scaleThresholdBeforeExtentTest"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("sendToClient"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slopeModAngle"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slopeModPercent"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slotDescriptorFilename"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("snapToTerrain"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("species"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("stepHeight"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("structureFootprintFileName"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("swimHeight"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("targetable"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("terrainModificationFileName"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("tintPalette"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("useStructureFootprintOutline"), bind(&ObjectVisitor::_handleBool, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("warpTolerance"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("waterModPercent"), bind(&ObjectVisitor::_handleFloat, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffect"), bind(&ObjectVisitor::_handleString, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("weaponEffectIndex"), bind(&ObjectVisitor::_handleInt, _1, _2, _3)));

		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("attackType"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("defaultValue"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("maxValueExclusive"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("minValueInclusive"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("palettePathName"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("defaultPaletteIndex"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("cameraHeight"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("variableName"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("sourceVariable"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("dependentVariable"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("gender"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("acceleration"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("speed"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("turnRate"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("postureAlignToTerrain"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionOffsetX"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("collisionOffsetZ"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("certificationsRequired"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("customizationVariableMapping"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("paletteColorCustomizationVariables"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("rangedIntCustomizationVariables"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("constStringCustomizationVariables"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("socketDestinations"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("surfaceType"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));

		//SCHEMATICS STUFF BELOW HERE
		
		//name is used for both "slots" and "attributes"
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("name"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		
		//slots marks the beginning of the slots section
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("slots"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("hardpoint"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		
		//attributes marks the beginning of the attributes section
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("attributes"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("experiment"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("value"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));

		//marks the end of the draft schematic.
		attributeHandler_.insert(make_pair<string, AttributeFunctor>(string("craftedSharedTemplate"), bind(&ObjectVisitor::_handleUnhandled, _1, _2, _3)));
	}
}

void ObjectVisitor::visit_folder(std::shared_ptr<folder_node> node)
{
}

void ObjectVisitor::visit_data(std::shared_ptr<file_node> node)
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

void ObjectVisitor::_handleXXXX(anh::ByteBuffer& buf)
{
	if(buf.size() > 0)
	{
		if(buf.peek<char>() == 1)
		{
			//This is a weird schematic edge case
		}
		else
		{
			std::string attributeName = buf.read<std::string>(false,true);
			AttributeHandlerIndexIterator it = attributeHandler_.find(attributeName);

			if(it != attributeHandler_.cend())
			{
				it->second(this, attributeName, buf);
			}
			else
			{
				printf("WARNING: \"%s\" is truly unhandled!\n", attributeName.c_str());
			}
		}
	}
}

void ObjectVisitor::_handleDERVXXXX(anh::ByteBuffer& buf)
{
	parentFiles.insert(buf.read<std::string>(false,true));
}

void ObjectVisitor::_handleClientString(ObjectVisitor* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		shared_ptr<ClientString> cs = make_shared<ClientString>();
		if(buf.read<char>())
		{
			cs->file = buf.read<std::string>(false,true);
			if(buf.read<char>())
			{
				cs->entry = buf.read<std::string>(false,true);
			}
		}
		dst->attributes_.insert(AttributeMap::value_type(move(name), std::make_shared<boost::any>(cs)));
	}
}

void ObjectVisitor::_handleString(ObjectVisitor* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->attributes_.insert(AttributeMap::value_type(move(name), std::make_shared<boost::any>(buf.read<std::string>(false,true))));
	}
}

void ObjectVisitor::_handleInt(ObjectVisitor* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		uint32_t buffer = buf.read<uint32_t>();
		dst->attributes_.insert(AttributeMap::value_type(move(name), std::make_shared<boost::any>(buffer)));
	}
}

void ObjectVisitor::_handleFloat(ObjectVisitor* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		buf.read<char>();
		float buffer = buf.read<float>();
		dst->attributes_.insert(AttributeMap::value_type(move(name), std::make_shared<boost::any>(buffer)));
	}
}

void ObjectVisitor::_handleBool(ObjectVisitor* dst, string& name, anh::ByteBuffer& buf)
{
	if(buf.read<char>())
	{
		dst->attributes_.insert(AttributeMap::value_type(move(name), std::make_shared<boost::any>((buf.read<char>()) ? true : false)));
	}
}

void ObjectVisitor::_handleUnhandled(ObjectVisitor* dst, std::string& name, anh::ByteBuffer& buf)
{
	//@TODO: PRINT A MESSAGE
}

void ObjectVisitor::debug()
{
	std::set<std::string>::iterator parentIt = parentFiles.begin();
	std::set<std::string>::iterator parentEnd = parentFiles.end();
	while(parentIt != parentEnd)
	{
		printf("Has Parent: %s\n", parentIt->c_str());
		++parentIt;
	}

	AttributeMap::const_iterator it = attributes_.cbegin();

	while(it != attributes_.cend())
	{
		printf("Attribute: \"%s\", value=", it->first.c_str(), it->second->type().name());
		if(it->second->type() == typeid(std::uint32_t))
		{
			printf("%d", boost::any_cast<std::uint32_t>(it->second));
		}
		else if(it->second->type() == typeid(shared_ptr<std::string>))
		{
			printf("%s", boost::any_cast<shared_ptr<std::string>>(it->second)->c_str());
		}
		else if(it->second->type() == typeid(shared_ptr<ClientString>))
		{
			shared_ptr<ClientString> cs = boost::any_cast<shared_ptr<ClientString>>(it->second);
			printf("@%s:%s", cs->file.c_str(), cs->entry.c_str());
		}
		else if(it->second->type() == typeid(float))
		{
			printf("%f", boost::any_cast<float>(it->second));
		}
		else if(it->second->type() == typeid(shared_ptr<bool>))
		{
			printf("%s", (boost::any_cast<bool>(it->second)) ? "true" : "false");
		}
		printf("\n");
		++it;
	}
}


void ObjectVisitor::load_aggregate_data(anh::resource::ResourceManagerInterface* f)
{
	if(!has_aggregate_)
	{
		AttributeMap aggregateAttributeMap;

		std::for_each(parentFiles.begin(), parentFiles.end(), [&] (std::string parentFile)
		{
			auto subI = std::static_pointer_cast<ObjectVisitor>(f->getResourceByName(parentFile, OIFF_VISITOR));
			subI->load_aggregate_data(f);

			//Now we continue to build up our map.
			std::for_each(subI->attributes_.begin(), subI->attributes_.end(), [&] (AttributeMap::value_type pair) {
				AttributeMap::iterator lb = aggregateAttributeMap.lower_bound(pair.first);
				if(lb != aggregateAttributeMap.end() && !(aggregateAttributeMap.key_comp()(pair.first, lb->first)))
				{
					lb->second = pair.second;
				}
				else
				{
					aggregateAttributeMap.insert(lb, AttributeMap::value_type(pair.first, pair.second));
				}
			});
		});

		std::for_each(attributes_.begin(), attributes_.end(), [&] (AttributeMap::value_type pair) {
			AttributeMap::iterator lb = aggregateAttributeMap.lower_bound(pair.first);
			if(lb != aggregateAttributeMap.end() && !(aggregateAttributeMap.key_comp()(pair.first, lb->first)))
			{
				lb->second = pair.second;
			}
			else
			{
				aggregateAttributeMap.insert(lb, AttributeMap::value_type(pair.first, pair.second));    // Use lb as a hint to insert,
																// so it can avoid another lookup
			}
		});

		attributes_ = std::move(aggregateAttributeMap);
		has_aggregate_ = true;
	}
}

void ObjectVisitor::load_referenced_files(anh::resource::ResourceManagerInterface* f)
{
	std::map<std::string, std::shared_ptr<boost::any>>::iterator itr;
	std::map<std::string, std::shared_ptr<boost::any>>::iterator end_itr = attributes_.end();

	//animationMapFilename
	//appearanceFilename
	//arrangementDescriptorFilename
	itr = attributes_.find("arrangementDescriptorFilename");
	if(itr != end_itr)
	{
		std::string value = boost::any_cast<std::string>(*(itr->second));
		if(value != "") {
			auto newVal = std::static_pointer_cast<SlotArrangementVisitor>(f->getResourceByName(value, SLOT_ARRANGEMENT_VISITOR));
			itr->second = std::make_shared<boost::any>(newVal);
		}
		else
		{
			itr->second = make_shared<boost::any>();
		}
	}

	//clientDataFile
	//cockpitFileName
	//interiorLayoutFileName
	//movementDatatable
	//portalLayoutFilename
	/*itr = attributes_.find("portalLayoutFilename");
	if(itr != end_itr)
	{
		itr->second = f.load<pob_interpreter>(itr->first);
	}*/

	//slotDescriptorFilename
	itr = attributes_.find("slotDescriptorFilename");
	if(itr != end_itr)
	{
		std::string value = boost::any_cast<std::string>(*(itr->second));
		if(value != "") {
			auto newVal = std::static_pointer_cast<SlotDescriptorVisitor>(f->getResourceByName(value, SLOT_DESCRIPTOR_VISITOR));
			itr->second = std::make_shared<boost::any>(newVal);
		}
		else
		{
			itr->second = make_shared<boost::any>();
		}
	}

	//structureFootprintFileName
	//terrainModificationFileName
}
