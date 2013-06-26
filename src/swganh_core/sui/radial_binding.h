#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/logger.h"
#include "swganh/scripting/python_shared_ptr.h"
#include "swganh/app/swganh_kernel.h"

#include <boost/python.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/call.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "radial_interface.h"

#include "swganh/scripting/utilities.h"
#include "swganh_core/object/creature/creature.h"

#include "swganh_core/messages/controllers/object_menu_request.h"
#include "swganh_core/messages/controllers/object_menu_response.h"



using namespace swganh::sui;
using namespace swganh::messages::controllers;
namespace bp = boost::python;
using namespace std;
using swganh::scripting::ScopedGilLock;

namespace swganh {
namespace sui {

	struct RadialWrap : RadialInterface, bp::wrapper<RadialInterface>
	{
		RadialWrap(PyObject* obj)
			: self_(bp::handle<>(bp::borrowed(obj)))
		{
			ScopedGilLock lock;
			bp::detail::initialize_wrapper(obj, this);
		}

		std::vector<RadialOptions> BuildRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, std::vector<RadialOptions> radials)
		{
			ScopedGilLock lock;
			try 
			{
				return this->get_override("buildRadial")(static_pointer_cast<swganh::object::Creature>(owner), target, radials);
			}
			catch (bp::error_already_set&)
			{
				swganh::scripting::logPythonException();
			}
			return std::vector<RadialOptions>();
		}
		void HandleRadial(std::shared_ptr<swganh::object::Object> owner, std::shared_ptr<swganh::object::Object> target, uint8_t action)
		{
			ScopedGilLock lock;
			try 
			{				
				this->get_override("handleRadial")(static_pointer_cast<swganh::object::Creature>(owner), target, action);
			}
			catch (bp::error_already_set&)
			{
				swganh::scripting::logPythonException();
			}
		}
	private:
		bp::object self_;
	};

	void exportRadial()
	{
		bp::class_<RadialInterface, RadialWrap, boost::noncopyable>("RadialMenu")
			.def("buildRadial", bp::pure_virtual(&RadialWrap::BuildRadial), "Builds a radial for the target :class:`Object`")
			.def("handleRadial", bp::pure_virtual(&RadialWrap::HandleRadial), "Handles a specific radial action")
			.def("getKernel", &RadialInterface::GetKernel, bp::return_internal_reference<>());

		bp::class_<RadialOptions>("RadialOptions", "class defining the options needed for radials", 
			bp::init<uint8_t, uint8_t, uint8_t, std::wstring>())
			.def_readwrite("parent_item", &RadialOptions::parent_item, "the parent item of this radial")
			.def_readwrite("identifier", &RadialOptions::identifier, "the :class:`RadialIndentifier` this radial is")
			.def_readwrite("action", &RadialOptions::action, "the action id this radial should apply to")
			.def_readwrite("description", &RadialOptions::custom_description, "only needed if action is 3");			
					
		bp::class_<std::vector<RadialOptions>>("RadialOptionsList", "vector for the radials options")
			.def(bp::vector_indexing_suite<std::vector<RadialOptions>>());

		bp::enum_<RadialIdentifier>("RadialIdentifier", "defines a radial")
			.value("unknown",Unknown)
			.value("combatTarget",combatTarget)
			.value("combatUntarget",combatUntarget)
			.value("combatAttack",combatAttack)
			.value("combatPeace",combatPeace)
			.value("combatDuel",combatDuel)
			.value("combatDeathBlow",combatDeathBlow)
			.value("examine",examine)
			.value("tradeStart",tradeStart)
			.value("tradeAccept",tradeAccept)
			.value("itemPickup",itemPickup)
			.value("itemEquip",itemEquip)
			.value("itemUnequip",itemUnequip)
			.value("itemDrop",itemDrop)
			.value("itemDestroy",itemDestroy)
			.value("itemToken",itemToken)
			.value("itemOpen",itemOpen)
			.value("itemOpenNewWindow",itemOpenNewWindow)
			.value("itemActivate",itemActivate)
			.value("itemDeactivate",itemDeactivate)
			.value("itemUse",itemUse)
			.value("itemUseSelf",itemUseSelf)
			.value("itemUseOther",itemUseOther)
			.value("itemSit",itemSit)
			.value("itemMail",itemMail)
			.value("converseStart",converseStart)
			.value("converseRespond",converseRespond)
			.value("converseResponse",converseResponse)
			.value("converseStop",converseStop)
			.value("craftOptions",craftOptions)
			.value("craftStart",craftStart)
			.value("craftHopperInput",craftHopperInput)
			.value("craftHopperOutput",craftHopperOutput)
			.value("missionTerminalList",missionTerminalList)
			.value("missionDetails",missionDetails)
			.value("loot",loot)
			.value("lootAll",lootAll)
			.value("groupInvite",groupInvite)
			.value("groupJoin",groupJoin)
			.value("groupLeave",groupLeave)
			.value("groupKick",groupKick)
			.value("groupDisband",groupDisband)
			.value("groupDecline",groupDecline)
			.value("extractObject",extractObject)
			.value("petCall",petCall)
			.value("terminalAuctionUse",terminalAuctionUse)
			.value("creatureFollow",creatureFollow)
			.value("creatureStopFollow",creatureStopFollow)
			.value("split",split)
			.value("imagedesign",imagedesign)
			.value("setName",setName)
			.value("itemRotate",itemRotate)
			.value("itemRotateRight",itemRotateRight)
			.value("itemRotateLeft",itemRotateLeft)
			.value("itemMove",itemMove)
			.value("itemMoveForward",itemMoveForward)
			.value("ItemMoveBack",ItemMoveBack)
			.value("itemMoveUp",itemMoveUp)
			.value("itemMoveDown",itemMoveDown)
			.value("petStore",petStore)
			.value("vehicleGenerate",vehicleGenerate)
			.value("vehicleStore",vehicleStore)
			.value("missionAbort",missionAbort)
			.value("missionEndDuty",missionEndDuty)
			.value("shipManageComponents",shipManageComponents)
			.value("waypointAutopilot",waypointAutopilot)
			.value("programDroid",programDroid)
			.value("serverDivider",serverDivider)
			.value("serverMenu1",serverMenu1)
			.value("serverMenu2",serverMenu2)
			.value("serverMenu3",serverMenu3)
			.value("serverMenu4",serverMenu4)
			.value("serverMenu5",serverMenu5)
			.value("serverMenu6",serverMenu6)
			.value("serverMenu7",serverMenu7)
			.value("serverMenu8",serverMenu8)
			.value("serverMenu9",serverMenu9)
			.value("serverMenu10",serverMenu10)
			.value("serverHarvesterManage",serverHarvesterManage)
			.value("serverHouseManage",serverHouseManage)
			.value("serverFactionHallManage",serverFactionHallManage)
			.value("serverHue",serverHue)
			.value("serverObserve",serverObserve)
			.value("serverStopObserving",serverStopObserving)
			.value("serverTravelOptions",serverTravelOptions)
			.value("serverBazaarOptions",serverBazaarOptions)
			.value("serverShippingOptions",serverShippingOptions)
			.value("serverHealWound",serverHealWound)
			.value("serverHealWoundHealth",serverHealWoundHealth)
			.value("serverHealWoundAction",serverHealWoundAction)
			.value("serverHealWoundStrength",serverHealWoundStrength)
			.value("serverHealWoundConst",serverHealWoundConst)
			.value("serverHealWoundQuickness",serverHealWoundQuickness)
			.value("serverHealWoundStamina",serverHealWoundStamina)
			.value("serverHealDamage",serverHealDamage)
			.value("serverHealState",serverHealState)
			.value("serverHealStateStunned",serverHealStateStunned)
			.value("serverHealStateBlinded",serverHealStateBlinded)
			.value("serverHealStateDizzy",serverHealStateDizzy)
			.value("serverHealStateIntim",serverHealStateIntim)
			.value("serverHealEnhance",serverHealEnhance)
			.value("serverHealEnhanceHealth",serverHealEnhanceHealth)
			.value("serverHealEnhanceAction",serverHealEnhanceAction)
			.value("serverHealEnhanceStrangth",serverHealEnhanceStrangth)
			.value("serverHealEnhanceConst",serverHealEnhanceConst)
			.value("serverHealEnhanceQuickness",serverHealEnhanceQuickness)
			.value("serverHealEnhanceStamina",serverHealEnhanceStamina)
			.value("serverHealFirstAid",serverHealFirstAid)
			.value("serverHealCurePoison",serverHealCurePoison)
			.value("serverHealCureDisease",serverHealCureDisease)
			.value("serverHealApplyPoison",serverHealApplyPoison)
			.value("serverHealApplyDisease",serverHealApplyDisease)
			.value("serverHarvestCorpse",serverHarvestCorpse)
			.value("serverPerformanceListen",serverPerformanceListen)
			.value("serverPerformanceWatch",serverPerformanceWatch)
			.value("serverPerformanceListenStop",serverPerformanceListenStop)
			.value("serverPerformanceWatchStop",serverPerformanceWatchStop)
			.value("serverTerminalPermissions",serverTerminalPermissions)
			.value("serverTerminalManagement",serverTerminalManagement)
			.value("serverTerminalPermissionsEnter",serverTerminalPermissionsEnter)
			.value("serverTerminalPermissionsBanned",serverTerminalPermissionsBanned)
			.value("serverTerminalPermissionsAdmin",serverTerminalPermissionsAdmin)
			.value("serverTerminalPermissionVendor",serverTerminalPermissionVendor)
			.value("serverTerminalPermissionsHopper",serverTerminalPermissionsHopper)
			.value("serverTerminalManagementStatus",serverTerminalManagementStatus)
			.value("serverTerminalManagementPrivacy",serverTerminalManagementPrivacy)
			.value("serverTerminalManagementTransfer",serverTerminalManagementTransfer)
			.value("serverTerminalManagementResidence",serverTerminalManagementResidence)
			.value("serverTerminalManagementDestroy",serverTerminalManagementDestroy)
			.value("serverTerminalManagementPay",serverTerminalManagementPay)
			.value("serverTerminalCreateVendor",serverTerminalCreateVendor)
			.value("serverGiveVendorMaintenance",serverGiveVendorMaintenance)
			.value("serverItemOptions",serverItemOptions)
			.value("serverSurveyToolRange",serverSurveyToolRange)
			.value("serverSurveyToolResolution",serverSurveyToolResolution)
			.value("serverSurveyToolClass",serverSurveyToolClass)
			.value("serverProbeDroidTrackTarget",serverProbeDroidTrackTarget)
			.value("serverProbeDroidFindTarget",serverProbeDroidFindTarget)
			.value("serverProbeDroidActivate",serverProbeDroidActivate)
			.value("serverProbeDroidBuy",serverProbeDroidBuy)
			.value("serverTeach",serverTeach)
			.value("petCommand",petCommand)
			.value("petFollow",petFollow)
			.value("petStay",petStay)
			.value("petGuard",petGuard)
			.value("petFriend",petFriend)
			.value("petAttack",petAttack)
			.value("petPatrol",petPatrol)
			.value("petGetPatrolPoint",petGetPatrolPoint)
			.value("petClearPatrolPoint ",petClearPatrolPoint )
			.value("petAssumeFormation1",petAssumeFormation1)
			.value("petAssumeFormation2",petAssumeFormation2)
			.value("petTransfer",petTransfer)
			.value("petRelease",petRelease)
			.value("petTrick1",petTrick1)
			.value("petTrick2",petTrick2)
			.value("petTrick3",petTrick3)
			.value("petTrick4",petTrick4)
			.value("petGroup",petGroup)
			.value("petTame",petTame)
			.value("petFeed",petFeed)
			.value("petSpecialAttackOne",petSpecialAttackOne)
			.value("petSpecialAttackTwo",petSpecialAttackTwo)
			.value("petRangedAttack",petRangedAttack)
			.value("diceRoll",diceRoll)
			.value("diceTwoFace",diceTwoFace)
			.value("diceThreeFace",diceThreeFace)
			.value("diceFourFace",diceFourFace)
			.value("diceFiveFace",diceFiveFace)
			.value("diceSixFace",diceSixFace)
			.value("diceSevenFace",diceSevenFace)
			.value("diceEightFace",diceEightFace)
			.value("diceCountOne",diceCountOne)
			.value("diceCountTwo",diceCountTwo)
			.value("diceCountThree",diceCountThree)
			.value("diceCountFour",diceCountFour)
			.value("createBallot",createBallot)
			.value("vote",vote)
			.value("bombingRun",bombingRun)
			.value("selfDestruct",selfDestruct)
			.value("thirtySec",thirtySec)
			.value("fifteenSec",fifteenSec)
			.value("serverCampDisband",serverCampDisband)
			.value("serverCampAssumeOwnership",serverCampAssumeOwnership)
			.value("serverProbeDroidProram",serverProbeDroidProram)
			.value("serverGuildCreate",serverGuildCreate)
			.value("serverGuildInfo",serverGuildInfo)
			.value("serverGuildMembers",serverGuildMembers)
			.value("serverGuildSponsored",serverGuildSponsored)
			.value("serverGuildEnemies",serverGuildEnemies)
			.value("serverGuildSponsor",serverGuildSponsor)
			.value("serverGuildDisband",serverGuildDisband)
			.value("serverGuildNameChange",serverGuildNameChange)
			.value("serverGuildGuildManagement",serverGuildGuildManagement)
			.value("serverGuildMemberManagement",serverGuildMemberManagement)
			.value("serverManfHopperInput",serverManfHopperInput)
			.value("serverManfHopperOutput",serverManfHopperOutput)
			.value("serverManfStationSchematic",serverManfStationSchematic)
			.value("elevatorUp",elevatorUp)
			.value("elevatorDown",elevatorDown)
			.value("serverPetOpen",serverPetOpen)
			.value("serverPetDpad",serverPetDpad)
			.value("serverMedToolDiagnose",serverMedToolDiagnose)
			.value("serverMedToolTendWound",serverMedToolTendWound)
			.value("serverMedToolTendDamage",serverMedToolTendDamage)
			.value("serverPetMount",serverPetMount)
			.value("serverPetDismount",serverPetDismount)
			.value("serverPetTrainMount",serverPetTrainMount)
			.value("serverVehicleEnter",serverVehicleEnter)
			.value("serverVehicleExit",serverVehicleExit)
			.value("openNaviCompDpad",openNaviCompDpad)
			.value("initNavicompDpad",initNavicompDpad)
			.value("cityStatus",cityStatus)
			.value("cityCitizens",cityCitizens)
			.value("cityStructures",cityStructures)
			.value("cityTreasury",cityTreasury)
			.value("cityManagement",cityManagement)
			.value("cityName",cityName)
			.value("cityMilitia",cityMilitia)
			.value("cityTaxes",cityTaxes)
			.value("cityTreasuryDeposit",cityTreasuryDeposit)
			.value("cityTreasuryWithdraw",cityTreasuryWithdraw)
			.value("cityRegister",cityRegister)
			.value("cityRank",cityRank)
			.value("cityAdmin1",cityAdmin1)
			.value("cityAdmin2",cityAdmin2)
			.value("cityAdmin3",cityAdmin3)
			.value("cityAdmin4",cityAdmin4)
			.value("cityAdmin5",cityAdmin5)
			.value("cityAdmin6",cityAdmin6)
			.value("memoryChipProgram",memoryChipProgram)
			.value("memoryChipTransfer",memoryChipTransfer)
			.value("memoryChipAnalyze",memoryChipAnalyze)
			.value("equipDroidOnChip",equipDroidOnChip)
			.value("bankJoin",bankJoin)
			.value("bankQuit",bankQuit)
			.value("bankDepositAll",bankDepositAll)
			.value("bankWithdrawAll",bankWithdrawAll)
			.value("bankTransfer",bankTransfer)
			.value("bankItems",bankItems)
			.value("fireworkshowAdd",fireworkshowAdd)
			.value("fireworkshowRemove",fireworkshowRemove)
			.value("fireworkshowModify",fireworkshowModify)
			.value("fireworkshowReorder",fireworkshowReorder)
			.value("fireworkshowData",fireworkshowData)
			.value("operateHarvester",operateHarvester)
			.value("payMaintenance",payMaintenance)
			.value("depositPower",depositPower)
			.value("StructureStatus",StructureStatus)
			.value("StructureOptions",StructureOptions)
			.value("StartManufacture",StartManufacture)
			.value("ListIngredients",ListIngredients)
			.value("StopManufacture", StopManufacture);
	}
}}
