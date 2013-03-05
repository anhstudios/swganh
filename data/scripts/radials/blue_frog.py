import swgpy
from swgpy.object import *
from swgpy.sui import *
from swgpy.utility import vector3, quat
import random

class PyRadialMenu(RadialMenu):
	
	def buildRadial(self, owner, target, radials):
		radial_list = RadialOptionsList()
		radial_list.append(RadialOptions(0, RadialIdentifier.itemUse, 1, 'Hack Universe'))
		radial_list.append(RadialOptions(0, RadialIdentifier.examine, 1, ''))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu1, 3, 'Vehicles (6 items)'))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu2, 3, 'Weapon Pack'))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu3, 3, 'Armor Pack'))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu4, 3, 'Structures Pack'))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu5, 3, 'Pets Pack'))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu6, 3, 'Droids Pack'))
		radial_list.append(RadialOptions(1, RadialIdentifier.serverMenu7, 3, 'Instrument Pack'))
		return radial_list
	
	levels = ('None', 'Light', 'Medium', 'Heavy')
	
	damage_types = ('Energy', 'Kinetic', 'Acid', 'Cold', 'Electricity', 'Heat')
	
	def defaultPostProcess(self, item):
		pass
	
	def weaponPostProcess(self, item):
		
		item.max_condition = random.randint(100, 10000)
		item.setStringAttribute('wpn_armor_pierce_rating', random.choice(self.levels))
		item.setFloatAttribute('wpn_attack_speed', random.uniform(0.1, 5))
		item.setStringAttribute('cat_wpn_damage.wpn_damage_type', random.choice(self.damage_types))
		min_damage = random.randint(1, 1000)
		max_damage = random.randint(min_damage, 1000)
		item.setIntAttribute('cat_wpn_damage.wpn_damage_min', min_damage)
		item.setIntAttribute('cat_wpn_damage.wpn_damage_max', max_damage)
		item.setFloatAttribute('cat_wpn_damage.wpn_wound_chance', random.uniform(0, 100))
		item.setIntAttribute('cat_wpn_rangemods.wpn_range_zero', 0)
		item.setIntAttribute('cat_wpn_rangemods.wpn_range_mid', 40)
		item.setIntAttribute('cat_wpn_rangemods.wpn_range_max', -80)
		item.setIntAttribute('cat_wpn_attack_cost.wpn_attack_cost_health', random.randint(1, 200))
		item.setIntAttribute('cat_wpn_attack_cost.wpn_attack_cost_action', random.randint(1, 200))
		item.setIntAttribute('cat_wpn_attack_cost.wpn_attack_cost_mind', random.randint(1, 200))
	
	def armorPostProcess(self, item):
		item.max_condition = random.randint(100, 10000)
		item.setStringAttribute('armor_rating', random.choice(self.levels))
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_kinetic', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_energy', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_blast', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_stun', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_elemental_heat', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_elemental_cold', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_elemental_acid', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_elemental_electrical', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_special_protection.armor_eff_restraint', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_restraint', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_energy', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_blast', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_stun', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_elemental_heat', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_elemental_cold', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_elemental_acid', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_elemental_electrical', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_effectiveness.armor_eff_restraint', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_kinetic', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_energy', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_blast', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_stun', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_elemental_heat', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_elemental_cold', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_elemental_acid', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_elemental_electrical', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setFloatAttribute('cat_armor_vulnerability.armor_eff_restraint', random.uniform(0, 100) if random.random()>0.7 else 0)
		item.setIntAttribute('cat_armor_encumbrance.armor_health_encumbrance', random.randint(20, 300))
		item.setIntAttribute('cat_armor_encumbrance.armor_action_encumbrance', random.randint(20, 300))
		item.setIntAttribute('cat_armor_encumbrance.armor_mind_encumbrance', random.randint(20, 300))
		item.setStringAttribute('crafter', 'Blue Frog, Inc.')
	
	def giveItems(self, owner, list, postProcess):
		sim = self.getKernel().serviceManager().simulationService()
		inv = self.getKernel().serviceManager().equipmentService().getEquippedObject(owner, "inventory")
		for name in list:
			item = sim.createObject(name, swgpy.ContainerPermission.DEFAULT)
			if item is not None:
				postProcess(item)
				inv.add(owner, item)
	
	def displaySUIList(self, owner, list, callbackName):
		sui = self.getKernel().serviceManager().suiService()
		
		#if sui.getSUIWindowByScriptName(owner, 'Script.listBox') != None:
			#return

		options = EventResultList()
		for option in list:
			options.append(option)
		
		window = sui.createListBox(ListBoxType.OK_CANCEL, '0xDEADBEEF', '...00010011010001...\n\n...[OVERRIDE]...\n\nWELCOME, JOHN SMEDLEY', options, owner)
			
		results = ResultList()
		results.append('List.lstList:SelectedRow')
		callback = PythonCallback(self, callbackName)
		
		window.subscribeToEventCallback(0, '', InputTrigger.OK, results, callback)
		window.subscribeToEventCallback(1, '', InputTrigger.CANCEL, results, callback)
		sui.openSUIWindow(window)
	
	def weaponCallback(self, owner, event_id, results):
		if event_id == 0:
			self.giveItems(owner, self.weapons[int(results[0])], self.weaponPostProcess)
		return True
	
	
	
	def armorCallback(self, owner, event_id, results):
		if event_id == 0:
			self.giveItems(owner, self.armor[int(results[0])], self.armorPostProcess)
		return True
		
	def structureCallback(self, owner, event_id, results):
		if event_id == 0:
			self.giveItems(owner, self.structureDeeds[int(results[0])], self.defaultPostProcess)
		return True
		
	def handleRadial(self, owner, target, action):
		if action == RadialIdentifier.serverMenu1:
			self.giveItems(owner, self.vehicleDeeds, self.defaultPostProcess)
		elif action == RadialIdentifier.serverMenu2:
			self.displaySUIList(owner, ['Melee Weapons', 'Ranged Weapons', 'Misc Weapons'], 'weaponCallback')
		elif action == RadialIdentifier.serverMenu3:
			self.displaySUIList(owner, ['Bone', 'Bounty Hunter', 'Chitin', 'Composite', 
			'Ithorian Defender', 'Ithorian Guardian', 'Ithorian Sentinel', 'Mandalorian', 
			'Marine', 'Padded', 'Ris', 'Stormtrooper', 'Tantel', 'Ubese'], 'armorCallback')
		elif action == RadialIdentifier.serverMenu4:
			self.displaySUIList(owner, ['Crafting Structures', 'Housing Structures', 'Corellia Civic Structures', 
			'Naboo Civic Structures', 'Tatooine Civic Structures', 'Guild Structures', 'Faction Structures'], 'structureCallback')
		elif action == RadialIdentifier.serverMenu5:
			self.giveItems(owner, self.petDeeds, self.defaultPostProcess)
		elif action == RadialIdentifier.serverMenu6:
			self.giveItems(owner, self.droidDeeds, self.defaultPostProcess)
		elif action == RadialIdentifier.serverMenu7:
			self.giveItems(owner, self.instruments, self.defaultPostProcess)
				
	vehicleDeeds = ('object/tangible/deed/vehicle_deed/shared_jetpack_deed.iff',
					'object/tangible/deed/vehicle_deed/shared_landspeeder_av21_deed.iff',
					'object/tangible/deed/vehicle_deed/shared_landspeeder_x31_deed.iff',
					'object/tangible/deed/vehicle_deed/shared_landspeeder_x34_deed.iff',
					'object/tangible/deed/vehicle_deed/shared_speederbike_flash_deed.iff',
					'object/tangible/deed/vehicle_deed/shared_speederbike_swoop_deed.iff')

	weapons = [('object/weapon/melee/2h_sword/shared_2h_sword_battleaxe.iff',
					'object/weapon/melee/2h_sword/shared_2h_sword_blacksun_hack.iff',
					'object/weapon/melee/2h_sword/shared_2h_sword_cleaver.iff',
					'object/weapon/melee/2h_sword/shared_2h_sword_katana.iff',
					'object/weapon/melee/2h_sword/shared_2h_sword_maul.iff',
					'object/weapon/melee/2h_sword/shared_2h_sword_scythe.iff',
					'object/weapon/melee/axe/shared_axe_heavy_duty.iff',
					'object/weapon/melee/axe/shared_axe_vibroaxe.iff',
					'object/weapon/melee/baton/shared_baton_gaderiffi.iff',
					'object/weapon/melee/baton/shared_baton_stun.iff',
					'object/weapon/melee/baton/shared_victor_baton_gaderiffi.iff',
					'object/weapon/melee/knife/shared_knife_dagger.iff',
					'object/weapon/melee/knife/shared_knife_donkuwah.iff',
					'object/weapon/melee/knife/shared_knife_janta.iff',
					'object/weapon/melee/knife/shared_knife_stone.iff',
					'object/weapon/melee/knife/shared_knife_stone_noob.iff',
					'object/weapon/melee/knife/shared_knife_survival.iff',
					'object/weapon/melee/knife/shared_knife_vibroblade.iff',
					'object/weapon/melee/polearm/shared_lance_nightsister.iff',
					'object/weapon/melee/polearm/shared_lance_staff_janta.iff',
					'object/weapon/melee/polearm/shared_lance_staff_metal.iff',
					'object/weapon/melee/polearm/shared_lance_staff_wood_s1.iff',
					'object/weapon/melee/polearm/shared_lance_staff_wood_s2.iff',
					'object/weapon/melee/polearm/shared_lance_vibrolance.iff',
					'object/weapon/melee/polearm/shared_polearm_vibro_axe.iff',
					'object/weapon/melee/special/shared_blacksun_razor.iff',
					'object/weapon/melee/special/shared_vibroknucler.iff'),
					
					('object/weapon/ranged/carbine/shared_carbine_cdef.iff',
					'object/weapon/ranged/carbine/shared_carbine_cdef_corsec.iff',
					'object/weapon/ranged/carbine/shared_carbine_dh17.iff',
					'object/weapon/ranged/carbine/shared_carbine_dh17_black.iff',
					'object/weapon/ranged/carbine/shared_carbine_dh17_snubnose.iff',
					'object/weapon/ranged/carbine/shared_carbine_dxr6.iff',
					'object/weapon/ranged/carbine/shared_carbine_e11.iff',
					'object/weapon/ranged/carbine/shared_carbine_ee3.iff',
					'object/weapon/ranged/carbine/shared_carbine_elite.iff',
					'object/weapon/ranged/carbine/shared_carbine_laser.iff',
					'object/weapon/ranged/carbine/shared_carbine_nym_slugthrower.iff',
					'object/weapon/ranged/heavy/shared_heavy_acid_beam.iff',
					'object/weapon/ranged/heavy/shared_heavy_lightning_beam.iff',
					'object/weapon/ranged/heavy/shared_heavy_particle_beam.iff',
					'object/weapon/ranged/heavy/shared_heavy_rocket_launcher.iff',
					'object/weapon/ranged/pistol/shared_pistol_cdef.iff',
					'object/weapon/ranged/pistol/shared_pistol_cdef_corsec.iff',
					'object/weapon/ranged/pistol/shared_pistol_cdef_noob.iff',
					'object/weapon/ranged/pistol/shared_pistol_d18.iff',
					'object/weapon/ranged/pistol/shared_pistol_de_10.iff',
					'object/weapon/ranged/pistol/shared_pistol_dh17.iff',
					'object/weapon/ranged/pistol/shared_pistol_dl44.iff',
					'object/weapon/ranged/pistol/shared_pistol_dl44_metal.iff',
					'object/weapon/ranged/pistol/shared_pistol_dx2.iff',
					'object/weapon/ranged/pistol/shared_pistol_fwg5.iff',
					'object/weapon/ranged/pistol/shared_pistol_geonosian_sonic_blaster_loot.iff',
					'object/weapon/ranged/pistol/shared_pistol_launcher.iff',
					'object/weapon/ranged/pistol/shared_pistol_power5.iff',
					'object/weapon/ranged/pistol/shared_pistol_republic_blaster.iff',
					'object/weapon/ranged/pistol/shared_pistol_scatter.iff',
					'object/weapon/ranged/pistol/shared_pistol_scout_blaster.iff',
					'object/weapon/ranged/pistol/shared_pistol_scout_blaster_corsec.iff',
					'object/weapon/ranged/pistol/shared_pistol_srcombat.iff',
					'object/weapon/ranged/pistol/shared_pistol_striker.iff',
					'object/weapon/ranged/pistol/shared_pistol_striker_noob.iff',
					'object/weapon/ranged/pistol/shared_pistol_tangle.iff',
					'object/weapon/ranged/rifle/shared_rifle_acid_beam.iff',
					'object/weapon/ranged/rifle/shared_rifle_beam.iff',
					'object/weapon/ranged/rifle/shared_rifle_berserker.iff',
					'object/weapon/ranged/rifle/shared_rifle_bowcaster.iff',
					'object/weapon/ranged/rifle/shared_rifle_cdef.iff',
					'object/weapon/ranged/rifle/shared_rifle_dlt20.iff',
					'object/weapon/ranged/rifle/shared_rifle_dlt20a.iff',
					'object/weapon/ranged/rifle/shared_rifle_e11.iff',
					'object/weapon/ranged/rifle/shared_rifle_ewok_crossbow.iff',
					'object/weapon/ranged/rifle/shared_rifle_flame_thrower.iff',
					'object/weapon/ranged/rifle/shared_rifle_jawa_ion.iff',
					'object/weapon/ranged/rifle/shared_rifle_laser.iff',
					'object/weapon/ranged/rifle/shared_rifle_laser_noob.iff',
					'object/weapon/ranged/rifle/shared_rifle_lightning.iff',
					'object/weapon/ranged/rifle/shared_rifle_sg82.iff',
					'object/weapon/ranged/rifle/shared_rifle_spraystick.iff',
					'object/weapon/ranged/rifle/shared_rifle_t21.iff',
					'object/weapon/ranged/rifle/shared_rifle_tenloss_dxr6_disruptor_loot.iff',
					'object/weapon/ranged/rifle/shared_rifle_tusken.iff',
					'object/weapon/ranged/rifle/shared_rifle_victor_tusken.iff'), 
					
					()]

	armor = [	('object/tangible/wearables/armor/bone/shared_armor_bone_s01_bicep_l.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_bicep_r.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_boots.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_bracer_l.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_bracer_r.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_chest_plate.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_gloves.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_helmet.iff',
				 'object/tangible/wearables/armor/bone/shared_armor_bone_s01_leggings.iff',),
				
				('object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_belt.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_bicep_l.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_bicep_r.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_boots.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_bracer_l.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_bracer_r.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_chest_plate.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_gloves.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_helmet.iff',
				 'object/tangible/wearables/armor/bounty_hunter/shared_armor_bounty_hunter_leggings.iff'),
				
				('object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_bicep_l.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_bicep_r.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_boots.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_bracer_l.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_bracer_r.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_chest_plate.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_gloves.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_helmet.iff',
				 'object/tangible/wearables/armor/chitin/shared_armor_chitin_s01_leggings.iff'),
				 
				('object/tangible/wearables/armor/composite/shared_armor_composite_bicep_l.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_bicep_r.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_boots.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_bracer_l.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_bracer_r.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_chest_plate.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_gloves.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_helmet.iff',
				 'object/tangible/wearables/armor/composite/shared_armor_composite_leggings.iff'),
				
				('object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_bicep_l.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_bicep_r.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_boots.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_bracer_l.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_bracer_r.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_chest_plate.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_gloves.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_helmet.iff',
				 'object/tangible/wearables/armor/ithorian_defender/shared_ith_armor_s01_leggings.iff'),
				
				('object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_bicep_l.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_bicep_r.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_boots.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_bracer_l.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_bracer_r.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_chest_plate.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_gloves.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_helmet.iff',
				 'object/tangible/wearables/armor/ithorian_guardian/shared_ith_armor_s02_leggings.iff'),
				
				('object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_bicep_l.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_bicep_r.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_boots.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_bracer_l.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_bracer_r.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_chest_plate.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_gloves.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_helmet.iff',
				 'object/tangible/wearables/armor/ithorian_sentinel/shared_ith_armor_s03_leggings.iff'),
				 
				('object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_belt.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_l.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bicep_r.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bracer_l.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_bracer_r.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_chest_plate.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_gloves.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_helmet.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_leggings.iff',
				 'object/tangible/wearables/armor/mandalorian/shared_armor_mandalorian_shoes.iff'),
				 
				('object/tangible/wearables/armor/marine/shared_armor_marine_backpack.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_bicep_l.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_bicep_r.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_boots.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_chest_plate.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_chest_plate_rebel.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_helmet.iff',
				 'object/tangible/wearables/armor/marine/shared_armor_marine_leggings.iff'),
				
				('object/tangible/wearables/armor/padded/shared_armor_padded_s01_belt.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_bicep_l.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_bicep_r.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_boots.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_bracer_l.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_bracer_r.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_chest_plate.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_gloves.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_helmet.iff',
				 'object/tangible/wearables/armor/padded/shared_armor_padded_s01_leggings.iff'),
				 
				('object/tangible/wearables/armor/ris/shared_armor_ris_bicep_l.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_bicep_r.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_boots.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_bracer_l.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_bracer_r.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_chest_plate.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_gloves.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_helmet.iff',
				 'object/tangible/wearables/armor/ris/shared_armor_ris_leggings.iff'),
				 
				('object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bicep_l.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bicep_r.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_boots.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bracer_l.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_bracer_r.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_chest_plate.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_gloves.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_helmet.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_leggings.iff',
				 'object/tangible/wearables/armor/stormtrooper/shared_armor_stormtrooper_utility_belt.iff'),
				 
				('object/tangible/wearables/armor/tantel/shared_armor_tantel_skreej_boots.iff',
				 'object/tangible/wearables/armor/tantel/shared_armor_tantel_skreej_chest_plate.iff',
				 'object/tangible/wearables/armor/tantel/shared_armor_tantel_skreej_helmet.iff'),
				
				('object/tangible/wearables/armor/ubese/shared_armor_ubese_bandolier.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_boots.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_bracer_l.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_bracer_r.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_gloves.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_helmet.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_jacket.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_pants.iff',
				 'object/tangible/wearables/armor/ubese/shared_armor_ubese_shirt.iff')]
					
	structureDeeds =  [('object/tangible/deed/factory_deed/shared_factory_clothing_deed.iff',
						'object/tangible/deed/factory_deed/shared_factory_food_deed.iff',
						'object/tangible/deed/factory_deed/shared_factory_item_deed.iff',
						'object/tangible/deed/factory_deed/shared_factory_structure_deed.iff',
						'object/tangible/deed/generator_deed/shared_generator_fusion_deed.iff',
						'object/tangible/deed/generator_deed/shared_generator_photo_bio_deed.iff',
						'object/tangible/deed/generator_deed/shared_generator_solar_deed.iff',
						'object/tangible/deed/generator_deed/shared_generator_wind_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_creature_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_flora_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_flora_deed_heavy.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_flora_deed_medium.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_gas_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_gas_deed_heavy.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_gas_deed_medium.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_liquid_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_liquid_deed_heavy.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_liquid_deed_medium.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_moisture_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_moisture_deed_heavy.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_moisture_deed_medium.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_ore_heavy_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_ore_s1_deed.iff',
						'object/tangible/deed/harvester_deed/shared_harvester_ore_s2_deed.iff'),
						
						('object/tangible/deed/player_house_deed/shared_corellia_house_large_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_large_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_medium_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_medium_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_small_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_small_floor_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_small_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_corellia_house_small_style_02_floor_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_large_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_large_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_medium_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_medium_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_small_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_small_floor_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_small_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_generic_house_small_style_02_floor_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_merchent_tent_style_01_deed.iff',
						'object/tangible/deed/player_house_deed/shared_merchent_tent_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_merchent_tent_style_03_deed.iff',
						'object/tangible/deed/player_house_deed/shared_naboo_house_large_deed.iff',
						'object/tangible/deed/player_house_deed/shared_naboo_house_medium_deed.iff',
						'object/tangible/deed/player_house_deed/shared_naboo_house_medium_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_naboo_house_small_deed.iff',
						'object/tangible/deed/player_house_deed/shared_naboo_house_small_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_tatooine_house_large_deed.iff',
						'object/tangible/deed/player_house_deed/shared_tatooine_house_medium_deed.iff',
						'object/tangible/deed/player_house_deed/shared_tatooine_house_medium_style_02_deed.iff',
						'object/tangible/deed/player_house_deed/shared_tatooine_house_small_deed.iff',
						'object/tangible/deed/player_house_deed/shared_tatooine_house_small_style_02_deed.iff'),
						
						('object/tangibe/deed/city_deed/shared_bank_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cityhall_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cloning_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garage_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_lrg_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_lrg_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_lrg_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_lrg_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_lrg_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_med_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_med_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_med_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_med_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_med_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_sml_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_sml_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_sml_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_sml_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_corellia_sml_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_hospital_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_shuttleport_corellia_deed.iff',
						 'object/tangibe/deed/city_deed/shared_theater_corellia_deed.iff'),
						 
						('object/tangibe/deed/city_deed/shared_bank_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cityhall_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cloning_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garage_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_lrg_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_lrg_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_lrg_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_lrg_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_lrg_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_med_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_med_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_med_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_med_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_med_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_sml_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_sml_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_sml_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_sml_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_naboo_sml_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_hospital_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_shuttleport_naboo_deed.iff',
						 'object/tangibe/deed/city_deed/shared_theater_naboo_deed.iff'),
						 
						('object/tangibe/deed/city_deed/shared_bank_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cityhall_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cloning_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garage_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_cantina_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_lrg_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_lrg_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_lrg_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_lrg_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_lrg_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_med_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_med_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_med_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_med_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_med_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_sml_01_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_sml_02_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_sml_03_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_sml_04_deed.iff',
						 'object/tangibe/deed/city_deed/shared_garden_tatooine_sml_05_deed.iff',
						 'object/tangibe/deed/city_deed/shared_hospital_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_shuttleport_tatooine_deed.iff',
						 'object/tangibe/deed/city_deed/shared_theater_tatooine_deed.iff'),
						 
						('object/tangible/deed/guild_deed/shared_corellia_guild_deed.iff',
						 'object/tangible/deed/guild_deed/shared_generic_guild_deed.iff',
						 'object/tangible/deed/guild_deed/shared_naboo_guild_deed.iff',
						 'object/tangible/deed/guild_deed/shared_tatooine_guild_deed.iff',
						 'object/tangible/deed/guild_deed/shared_tatooine_guild_style_02_deed.iff'),
						 
						('object/tangible/deed/faction_perk/covert_detector/shared_detector_32m_deed.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s01.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s01_pvp.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s02.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s02_pvp.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s03.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s03_pvp.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s04.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s04_pvp.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s05.iff',
						 'object/tangible/deed/faction_perk/hq/shared_hq_s05_pvp.iff',
						 'object/tangible/deed/faction_perk/minefield/shared_field_1x1_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_block_lg_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_block_med_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_block_sm_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_dish_lg_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_dish_sm_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_tower_lg_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_tower_med_deed.iff',
						 'object/tangible/deed/faction_perk/turret/shared_tower_sm_deed.iff')]
					
	petDeeds = ('object/tangible/deed/pet_deed/shared_angler_deed.iff',
				'object/tangible/deed/pet_deed/shared_bageraset_deed.iff',
				'object/tangible/deed/pet_deed/shared_bantha_deed.iff',
				'object/tangible/deed/pet_deed/shared_bearded_jax_deed.iff',
				'object/tangible/deed/pet_deed/shared_blurrg_deed.iff',
				'object/tangible/deed/pet_deed/shared_boar_wolf_deed.iff',
				'object/tangible/deed/pet_deed/shared_bocatt_deed.iff',
				'object/tangible/deed/pet_deed/shared_bol_deed.iff',
				'object/tangible/deed/pet_deed/shared_bolle_bol_deed.iff',
				'object/tangible/deed/pet_deed/shared_bolma_deed.iff',
				'object/tangible/deed/pet_deed/shared_bordok_deed.iff',
				'object/tangible/deed/pet_deed/shared_brackaset_deed.iff',
				'object/tangible/deed/pet_deed/shared_carrion_spat_deed.iff',
				'object/tangible/deed/pet_deed/shared_choku_deed.iff',
				'object/tangible/deed/pet_deed/shared_cu_pa_deed.iff',
				'object/tangible/deed/pet_deed/shared_dalyrake_deed.iff',
				'object/tangible/deed/pet_deed/shared_dewback_deed.iff',
				'object/tangible/deed/pet_deed/shared_dune_lizard_deed.iff',
				'object/tangible/deed/pet_deed/shared_durni_deed.iff',
				'object/tangible/deed/pet_deed/shared_eopie_deed.iff',
				'object/tangible/deed/pet_deed/shared_falumpaset_deed.iff',
				'object/tangible/deed/pet_deed/shared_fambaa_deed.iff',
				'object/tangible/deed/pet_deed/shared_gnort_deed.iff',
				'object/tangible/deed/pet_deed/shared_graul_deed.iff',
				'object/tangible/deed/pet_deed/shared_gronda_deed.iff',
				'object/tangible/deed/pet_deed/shared_gualama_deed.iff',
				'object/tangible/deed/pet_deed/shared_guf_drolg_deed.iff',
				'object/tangible/deed/pet_deed/shared_gurnaset_deed.iff',
				'object/tangible/deed/pet_deed/shared_gurrcat_deed.iff',
				'object/tangible/deed/pet_deed/shared_gurreck_deed.iff',
				'object/tangible/deed/pet_deed/shared_hermit_spider_deed.iff',
				'object/tangible/deed/pet_deed/shared_huf_dun_deed.iff',
				'object/tangible/deed/pet_deed/shared_huurton_deed.iff',
				'object/tangible/deed/pet_deed/shared_ikopi_deed.iff',
				'object/tangible/deed/pet_deed/shared_kaadu_deed.iff',
				'object/tangible/deed/pet_deed/shared_kahmurra_deed.iff',
				'object/tangible/deed/pet_deed/shared_kima_deed.iff',
				'object/tangible/deed/pet_deed/shared_kimogila_deed.iff',
				'object/tangible/deed/pet_deed/shared_kliknik_deed.iff',
				'object/tangible/deed/pet_deed/shared_krahbu_deed.iff',
				'object/tangible/deed/pet_deed/shared_kusak_deed.iff',
				'object/tangible/deed/pet_deed/shared_kwi_deed.iff',
				'object/tangible/deed/pet_deed/shared_langlatch_deed.iff',
				'object/tangible/deed/pet_deed/shared_malkloc_deed.iff',
				'object/tangible/deed/pet_deed/shared_mawgax_deed.iff',
				'object/tangible/deed/pet_deed/shared_marek_deed.iff',
				'object/tangible/deed/pet_deed/shared_mott_deed.iff',
				'object/tangible/deed/pet_deed/shared_narglatch_deed.iff',
				'object/tangible/deed/pet_deed/shared_piket_deed.iff',
				'object/tangible/deed/pet_deed/shared_pugoriss_deed.iff',
				'object/tangible/deed/pet_deed/shared_rancor_deed.iff',
				'object/tangible/deed/pet_deed/shared_roba_deed.iff',
				'object/tangible/deed/pet_deed/shared_ronto_deed.iff',
				'object/tangible/deed/pet_deed/shared_sand_panther_deed.iff',
				'object/tangible/deed/pet_deed/shared_sharnaff_deed.iff',
				'object/tangible/deed/pet_deed/shared_shear_mite_deed.iff',
				'object/tangible/deed/pet_deed/shared_slice_hound_deed.iff',
				'object/tangible/deed/pet_deed/shared_snorbal_deed.iff',
				'object/tangible/deed/pet_deed/shared_squall_deed.iff',
				'object/tangible/deed/pet_deed/shared_swirl_prong_deed.iff',
				'object/tangible/deed/pet_deed/shared_thune_deed.iff',
				'object/tangible/deed/pet_deed/shared_torton_deed.iff',
				'object/tangible/deed/pet_deed/shared_tybis_deed.iff',
				'object/tangible/deed/pet_deed/shared_veermok_deed.iff',
				'object/tangible/deed/pet_deed/shared_verne_deed.iff',
				'object/tangible/deed/pet_deed/shared_vesp_deed.iff',
				'object/tangible/deed/pet_deed/shared_vir_vur_deed.iff',
				'object/tangible/deed/pet_deed/shared_woolamander_deed.iff',
				'object/tangible/deed/pet_deed/shared_zucca_boar_deed.iff')
	
	droidDeeds = (	'object/tangible/deed/pet_deed/shared_deed_3p0_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_3p0_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_binary_load_lifter_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_binary_load_lifter_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_dz70_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_dz70_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_le_repair_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_le_repair_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_mse_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_mse_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_power_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_power_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_probot_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_probot_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r2_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r2_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r3_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r3_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r4_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r4_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r5_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_r5_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_surgical_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_surgical_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_treadwell_advanced_basic.iff',
					'object/tangible/deed/pet_deed/shared_deed_treadwell_basic.iff',
					)
					
	instruments = ( 'object/tangible/instrument/shared_bandfill.iff',
					'object/tangible/instrument/shared_fanfar.iff',
					'object/tangible/instrument/shared_fizz.iff',
					'object/tangible/instrument/shared_flute_droopy.iff',
					'object/tangible/instrument/shared_instrument_kloo_horn.iff',
					'object/tangible/instrument/shared_mandoviol.iff',
					'object/tangible/instrument/shared_nalargon.iff',
					'object/tangible/instrument/shared_ommni_box.iff',
					'object/tangible/instrument/shared_slitherhorn.iff',
					'object/tangible/instrument/shared_traz.iff')