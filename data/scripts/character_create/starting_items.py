
def GetStartingItems(species, profession, gender):
	#Grab items common to all
	itemList = list(CommonItems)
	
	#Grab items based on species and profession
	if species == 'wookiee' or species == 'ithorian':
		itemList.extend(StartingItems[species+":"+gender + ':' + profession])
	else:
		itemList.extend(StartingItems[gender+":"+profession])
	
	# hack remove shoes/boots for trandoshans
	if species == 'trandoshan':
		for i in range(len(itemList)):
			if itemList[i].find('shoe') != -1:
				itemList.pop(i)
			elif itemList[i].find('boot') != -1:
				itemList.pop(i)
				
	return itemList

#Ithorian
def GetIthorianItems(profession, gender):
	return 
	
StartingItems = {
	'female:social_entertainer' : 	(	
										'object/tangible/instrument/shared_slitherhorn.iff', 
										'object/tangible/wearables/shirt/shared_shirt_s38.iff',
										'object/tangible/wearables/pants/shared_pants_s11.iff',
										'object/tangible/wearables/shoes/shared_shoes_s09.iff'
									),
									
	'male:social_entertainer' : 	(	
										'object/tangible/instrument/shared_slitherhorn.iff', 
										'object/tangible/wearables/shirt/shared_shirt_s42.iff',
										'object/tangible/wearables/pants/shared_pants_s08.iff',
										'object/tangible/wearables/shoes/shared_shoes_s01.iff'
									),
									
	'female:science_medic' : 		(	
										'object/tangible/crafting/station/shared_food_tool.iff',
										'object/tangible/wearables/bodysuit/shared_bodysuit_s13.iff',
										'object/tangible/wearables/boots/shared_boots_s04.iff'
									),
										
	'male:science_medic' : 			(	
										'object/tangible/crafting/station/shared_food_tool.iff',
										'object/tangible/wearables/jacket/shared_jacket_s24.iff',
										'object/tangible/wearables/shirt/shared_shirt_s10.iff',
										'object/tangible/wearables/pants/shared_pants_s12.iff',
										'object/tangible/wearables/shoes/shared_shoes_s02.iff'
									),
										
	'female:outdoors_scout' : 		(	
										'object/tangible/crafting/station/shared_generic_tool.iff',
										'object/tangible/wearables/shoes/shared_shoes_s02.iff',
										'object/tangible/wearables/shirt/shared_shirt_s26.iff',
										'object/tangible/wearables/pants/shared_pants_s21.iff'
									),
	
	'male:outdoors_scout' : 		(	
										'object/tangible/crafting/station/shared_generic_tool.iff', 
										'object/tangible/wearables/shoes/shared_shoes_s02.iff', 
										'object/tangible/wearables/shirt/shared_shirt_s16.iff',
										'object/tangible/wearables/pants/shared_pants_s21.iff',
										'object/tangible/wearables/vest/shared_vest_s09.iff'
									),
	
	'female:crafting_artisan' : 	(	
										'object/tangible/crafting/station/shared_generic_tool.iff',
										'object/tangible/survey_tool/shared_survey_tool_mineral.iff', 
										'object/tangible/wearables/shirt/shared_shirt_s12.iff',
										'object/tangible/wearables/skirt/shared_skirt_s10.iff',
										'object/tangible/wearables/shoes/shared_shoes_s07.iff'
									),
	
	'male:crafting_artisan' : 			(	
											'object/tangible/crafting/station/shared_generic_tool.iff',
											'object/tangible/survey_tool/shared_survey_tool_mineral.iff', 
											'object/tangible/wearables/shirt/shared_shirt_s34.iff',
											'object/tangible/wearables/pants/shared_pants_s29.iff',
											'object/tangible/wearables/shoes/shared_shoes_s01.iff'
										),
	
	'female:combat_brawler' : 			(	
											'object/tangible/wearables/shirt/shared_shirt_s27.iff',
											'object/tangible/wearables/pants/shared_pants_s07.iff',
											'object/tangible/wearables/boots/shared_boots_s05.iff',
											'object/tangible/wearables/gloves/shared_gloves_s03.iff'
										),
	
	'male:combat_brawler' : 			(	
											'object/tangible/wearables/shirt/shared_shirt_s14.iff',
											'object/tangible/wearables/jacket/shared_jacket_s16.iff',
											'object/tangible/wearables/pants/shared_pants_s14.iff',
											'object/tangible/wearables/boots/shared_boots_s05.iff'
										),
	
	'female:combat_marksman' : 			(	
											'object/tangible/wearables/shirt/shared_shirt_s24.iff',
											'object/tangible/wearables/pants/shared_pants_s24.iff',
											'object/tangible/wearables/vest/shared_vest_s10.iff',
											'object/tangible/wearables/shoes/shared_shoes_s02.iff'
										),
	
	'male:combat_marksman' : 			(	
											'object/tangible/wearables/jacket/shared_jacket_s05.iff',
											'object/tangible/wearables/pants/shared_pants_s04.iff',
											'object/tangible/wearables/boots/shared_boots_s21.iff'
										),

	'ithorian:female:social_entertainer' : (
											'object/tangible/instrument/shared_slitherhorn.iff', 
											'object/tangible/wearables/ithorian/shared_ith_pants_s11.iff',
											'object/tangible/wearables/ithorian/shared_ith_shirt_s09.iff'
										),
	
	'ithorian:male:social_entertainer' : (	
											'object/tangible/instrument/shared_slitherhorn.iff', 
											'object/tangible/wearables/ithorian/shared_ith_pants_s08.iff',
											'object/tangible/wearables/ithorian/shared_ith_shirt_s01.iff'
										),
	
	'ithorian:female:outdoors_scout' : 	(	
											'object/tangible/crafting/station/shared_generic_tool.iff', 
											'object/tangible/wearables/ithorian/shared_ith_shirt_s04.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s21.iff'
										),
	
	'ithorian:male:outdoors_scout' : 	(
											'object/tangible/crafting/station/shared_generic_tool.iff', 
											'object/tangible/wearables/ithorian/shared_ith_shirt_s04.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s21.iff'
										),
	
	'ithorian:female:science_medic' : 	(
											'object/tangible/crafting/station/shared_food_tool.iff',
											'object/tangible/wearables/ithorian/shared_ith_shirt_s01.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s03.iff'
										),
	
	'ithorian:male:science_medic' : 	(
											'object/tangible/crafting/station/shared_food_tool.iff',
											'object/tangible/wearables/ithorian/shared_ith_jacket_s02.iff',
											'object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s12.iff'
										),
	
	'ithorian:female:crafting_artisan' :(
											'object/tangible/crafting/station/shared_generic_tool.iff',
											'object/tangible/survey_tool/shared_survey_tool_mineral.iff', 
											'object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s06.iff',
											'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff'
										),
	
	'ithorian:male:crafting_artisan' : 	(
											'object/tangible/crafting/station/shared_generic_tool.iff',
											'object/tangible/survey_tool/shared_survey_tool_mineral.iff', 
											'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff',
											'object/tangible/wearables/ithorian/shared_ith_shirt_s10.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s06.iff'
										),
	
	'ithorian:female:combat_brawler' : 	(
											'object/tangible/wearables/ithorian/shared_ith_shirt_s02.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s07.iff',
											'object/tangible/wearables/ithorian/shared_ith_gloves_s01.iff'
										),
	
	'ithorian:male:combat_brawler' : 	(
											'object/tangible/wearables/ithorian/shared_ith_shirt_s02.iff',
											'object/tangible/wearables/ithorian/shared_ith_jacket_s16.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s04.iff'
										),
	
	'ithorian:female:combat_marksman' : (
											'object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s06.iff',
											'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff'
										),
	
	'ithorian:male:combat_marksman' : 	(
											'object/tangible/wearables/ithorian/shared_ith_jacket_s05.iff',
											'object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff',
											'object/tangible/wearables/ithorian/shared_ith_pants_s04.iff'
										),
										
	'wookiee:female:crafting_artisan' : (
											'object/tangible/crafting/station/shared_generic_tool.iff',
											'object/tangible/survey_tool/shared_survey_tool_mineral.iff', 
										),
										
	'wookiee:male:crafting_artisan' : (
											'object/tangible/crafting/station/shared_generic_tool.iff',
											'object/tangible/survey_tool/shared_survey_tool_mineral.iff', 
										),
	
	'wookiee:female:social_entertainer' : (
											'object/tangible/instrument/shared_slitherhorn.iff', 
										),
										
	'wookiee:male:social_entertainer' : (
											'object/tangible/instrument/shared_slitherhorn.iff', 
										),

	'wookiee:female:science_medic' : 	(
											'object/tangible/crafting/station/shared_food_tool.iff',
										),
										
	'wookiee:male:science_medic' : 		(
											'object/tangible/crafting/station/shared_food_tool.iff',
										),
	
	'wookiee:female:outdoors_scout' : 	(
											'object/tangible/crafting/station/shared_generic_tool.iff', 
										),
	'wookiee:male:outdoors_scout' : 	(
											'object/tangible/crafting/station/shared_generic_tool.iff', 
										),
										
	'wookiee:female:combat_brawler' : 	(),
	'wookiee:male:combat_brawler' : 	(),
	'wookiee:female:combat_marksman' : 	(),
	'wookiee:male:combat_marksman' : 	(),
}

#All Get These
CommonItems = 						(
										'object/tangible/food/shared_fruit_melon.iff',
										'object/weapon/ranged/pistol/shared_pistol_cdef_noob.iff',
										'object/weapon/melee/knife/shared_knife_survival.iff'
									)