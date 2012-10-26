
def GetStartingItems(species, profession, gender):
	ItemList = CommonItems
	if species == 'wookiee':
		GetWookieeItems(ItemList, profession, gender)
	elif species == 'ithorian':
		GetIthorianItems(ItemList, profession, gender)
	else:
		if profession == 'social_entertainer':
			GetEntertainerItems(ItemList, gender)
		elif profession == 'science_medic':
			GetMedicItems(ItemList, gender)
		elif profession == 'outdoors_scout':
			GetScoutItems(ItemList, gender)
		elif profession == 'crafting_artisan':
			GetArtisanItems(ItemList, gender)
		elif profession == 'combat_brawler':
			GetBrawlerItems(ItemList, gender)
		elif profession == 'combat_marksman':
			GetMarksmanItems(ItemList, gender)
	
	# hack remove shoes/boots...
	if species == 'trandoshan':
		print('item list before filter, trandoshan')
		print(ItemList)
		filter (lambda a: 'shoe' in a, ItemList)
		filter (lambda a: 'boot' in a, ItemList)
		print('item list after filter')
		print(ItemList)
	return ItemList
		

# trandoshans dont get shoes...
#Profession Specific Items
def GetEntertainerItems(ItemList, gender):
	ItemList.append('object/tangible/instrument/shared_slitherhorn.iff')
	if (gender = 'female'):
		#female
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s38.iff',
		'object/tangible/wearables/pants/shared_pants_s11.iff',
		'object/tangible/wearables/shoes/shared_shoes_s09.iff'
		}
	else:
		#male
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s42.iff',
		'object/tangible/wearables/pants/shared_pants_s08.iff',
		'object/tangible/wearables/shoes/shared_shoes_s01.iff'
		}
def GetMedicItems(ItemList, gender):	
	#science_medic
	ItemList.append('object/tangible/crafting/station/shared_food_tool.iff')
	if (gender = 'female'):
		#female
		ItemList += {
		'object/tangible/wearables/bodysuit/shared_bodysuit_s13.iff',
		'object/tangible/wearables/boots/shared_boots_s04.iff'
		}
	else:		
		#male
		ItemList += {
		'object/tangible/wearables/jacket/shared_jacket_s24.iff',
		'object/tangible/wearables/shirt/shared_shirt_s10.iff',
		'object/tangible/wearables/pants/shared_pants_s12.iff',
		'object/tangible/wearables/shoes/shared_shoes_s02.iff'
		}
def GetScoutItems(ItemList, gender):
	#scout
	ItemList += {
	'object/tangible/crafting/station/shared_generic_tool.iff',
	'object/tangible/wearables/shoes/shared_shoes_s02.iff'
	}
	if gender == 'female':
		#female
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s26.iff',
		'object/tangible/wearables/pants/shared_pants_s21.iff'
		}
	else:
		#male
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s16.iff',
		'object/tangible/wearables/pants/shared_pants_s21.iff'
		}
		
def GetArtisanItems(ItemList, gender):
	#artisan
	ItemList += {
	'object/tangible/crafting/station/shared_generic_tool.iff',
	'object/tangible/survey_tool/shared_survey_tool_mineral.iff'
	}
	if gender == 'female':
		#female
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s12.iff',
		'object/tangible/wearables/skirt/shared_skirt_s10.iff',
		'object/tangible/wearables/shoes/shared_shoes_s07.iff'
		}
	else:	
		#male
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s34.iff',
		'object/tangible/wearables/pants/shared_pants_s29.iff',
		'object/tangible/wearables/shoes/shared_shoes_s01.iff',
		}
def GetBrawlerItems(ItemList, gender):
	#brawler
	if gender == 'female':
		#female
		ItemList = {
		'object/tangible/wearables/shirt/shared_shirt_s27.iff',
		'object/tangible/wearables/pants/shared_pants_s07.iff',
		'object/tangible/wearables/boots/shared_boots_s05.iff',
		'object/tangible/wearables/gloves/shared_gloves_s03.iff'
		}
	else:
		#male
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s14.iff',
		'object/tangible/wearables/jacket/shared_jacket_s16.iff',
		'object/tangible/wearables/pants/shared_pants_s14.iff',
		'object/tangible/wearables/boots/shared_boots_s05.iff'
		}
def GetMarskmanItems(ItemList, gender):
	if gender == 'female':
		#female
		ItemList += {
		'object/tangible/wearables/shirt/shared_shirt_s24.iff',
		'object/tangible/wearables/pants/shared_pants_s24.iff',
		'object/tangible/wearables/vest/shared_vest_s10.iff',
		'object/tangible/wearables/shoes/shared_shoes_s02.iff'
		}
	else:
		#male	
		ItemList += {
		'object/tangible/wearables/jacket/shared_jacket_s05.iff',
		'object/tangible/wearables/pants/shared_pants_s04.iff',
		'object/tangible/wearables/boots/shared_boots_s21.iff'
		}
	
def GetWookieItems(ItemList, profession, gender)
	ItemList += ProfessionItems[profession]
	if profession = 'science_medic':
		ItemList.append('object/tangible/wearables/jacket/shared_jacket_s24.iff')

#Ithorian
def GetIthorianItems(ItemList, profession, gender):
	ItemList += IthorianItems[gender + ':' + profession]
	ItemList += ProfessionItems[profession]
	
IthorianItems = {
	'female:social_entertainer' : {'object/tangible/wearables/ithorian/shared_ith_pants_s11.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s09.iff'},
	'male:social_entertainer' : {'object/tangible/wearables/ithorian/shared_ith_pants_s08.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s01.iff'},
	'female:outdoors_scout' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s04.iff','object/tangible/wearables/ithorian/shared_ith_pants_s21.iff'},
	'male:outdoors_scout' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s04.iff','object/tangible/wearables/ithorian/shared_ith_pants_s21.iff'},
	'female:science_medic' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s01.iff','object/tangible/wearables/ithorian/shared_ith_pants_s03.iff'},
	'male:science_medic' : {'object/tangible/wearables/ithorian/shared_ith_jacket_s02.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s12.iff'},
	'female:crafting_artisan' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s06.iff', 'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff'},
	'male:crafting_artisan' : {'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s10.iff','object/tangible/wearables/ithorian/shared_ith_pants_s06.iff'},
	'female:combat_brawler' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s02.iff','object/tangible/wearables/ithorian/shared_ith_pants_s07.iff', 'object/tangible/wearables/ithorian/shared_ith_gloves_s01.iff'},
	'male:combat_brawler' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s02.iff','object/tangible/wearables/ithorian/shared_ith_jacket_s16.iff','object/tangible/wearables/ithorian/shared_ith_pants_s04.iff'},
	'female:combat_marksman' : {'object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s06.iff', 'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff'},
	'male:combat_brawler' : {'object/tangible/wearables/ithorian/shared_ith_jacket_s05.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s04.iff'}
}
#All Get These
CommonItems = {
	'object/tangible/food/shared_fruit_melon.iff',
	'object/weapon/ranged/pistol/shared_pistol_cdef.iff',
	'object/weapon/melee/knife/shared_knife_survival.iff'
}