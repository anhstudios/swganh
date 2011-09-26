/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_

namespace swganh {
namespace object {
namespace creature {

class Creature;
class CreatureMessageBuilder
{
public:
    
    // deltas
    static void BuildBankCreditsDelta(Creature* creature);
    static void BuildCashCreditsDelta(Creature* creature);
    static void BuildSkillListDelta(Creature* creature, uint8_t subtype, std::string skill);
    static void BuildPostureDelta(Creature* creature);
    static void BuildFactionRankDelta(Creature* creature);
    static void BuildOwnerIdDelta(Creature* creature);
    static void BuildScaleDelta(Creature* creature);
    static void BuildBattleFatigueDelta(Creature* creature);
    static void BuildStateBitmaskDelta(Creature* creature);

    // baselines
};

}}} // swganh::object::creature

#endif // SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_