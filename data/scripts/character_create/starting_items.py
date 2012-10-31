
def GetStartingItems(species, profession, gender):
	itemList = CommonItems
	if profession in ProfessionItems:
		itemList += ProfessionItems[profession]
	if species == 'wookiee':
		GetWookieeItems(itemList, profession, gender)
	elif species == 'ithorian':
		GetIthorianItems(itemList, profession, gender)
	else:
		if profession == 'social_entertainer':
			GetEntertainerItems(itemList, gender)
		elif profession == 'science_medic':
			GetMedicItems(itemList, gender)
		elif profession == 'outdoors_scout':
			GetScoutItems(itemList, gender)
		elif profession == 'crafting_artisan':
			GetArtisanItems(itemList, gender)
		elif profession == 'combat_brawler':
			GetBrawlerItems(itemList, gender)
		elif profession == 'combat_marksman':
			GetMarksmanItems(itemList, gender)
	
	# hack remove shoes/boots...
	if species == 'trandoshan':
		for i in range(len(itemList)):
			if itemList[i].find('shoe') != -1:
				itemList.pop(i)
			elif itemList[i].find('boot') != -1:
				itemList.pop(i)
	return itemList
		

# trandoshans dont get shoes...
#Profession Specific Items
def GetEntertainerItems(itemList, gender):
	if (gender == 'female'):
		#female
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s38.iff',
		'object/tangible/wearables/pants/shared_pants_s11.iff',
		'object/tangible/wearables/shoes/shared_shoes_s09.iff'
		]
	else:
		#male
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s42.iff',
		'object/tangible/wearables/pants/shared_pants_s08.iff',
		'object/tangible/wearables/shoes/shared_shoes_s01.iff'
		]
def GetMedicItems(itemList, gender):	
	#science_medic
	if (gender == 'female'):
		#female
		itemList += [
		'object/tangible/wearables/bodysuit/shared_bodysuit_s13.iff',
		'object/tangible/wearables/boots/shared_boots_s04.iff'
		]
	else:		
		#male
		itemList += [
		'object/tangible/wearables/jacket/shared_jacket_s24.iff',
		'object/tangible/wearables/shirt/shared_shirt_s10.iff',
		'object/tangible/wearables/pants/shared_pants_s12.iff',
		'object/tangible/wearables/shoes/shared_shoes_s02.iff'
		]
def GetScoutItems(itemList, gender):
	#scout
	if gender == 'female':
		#female
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s26.iff',
		'object/tangible/wearables/pants/shared_pants_s21.iff'		
		]
	else:
		#male
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s16.iff',
		'object/tangible/wearables/pants/shared_pants_s21.iff',
		'object/tangible/wearables/vest/shared_vest_s09.iff'
		]
		
def GetArtisanItems(itemList, gender):
	# artisan
	if gender == 'female':
		#female
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s12.iff',
		'object/tangible/wearables/skirt/shared_skirt_s10.iff',
		'object/tangible/wearables/shoes/shared_shoes_s07.iff'
		]
	else:	
		#male
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s34.iff',
		'object/tangible/wearables/pants/shared_pants_s29.iff',
		'object/tangible/wearables/shoes/shared_shoes_s01.iff',
		]
def GetBrawlerItems(itemList, gender):
	#brawler
	if gender == 'female':
		#female
		itemList = [
		'object/tangible/wearables/shirt/shared_shirt_s27.iff',
		'object/tangible/wearables/pants/shared_pants_s07.iff',
		'object/tangible/wearables/boots/shared_boots_s05.iff',
		'object/tangible/wearables/gloves/shared_gloves_s03.iff'
		]
	else:
		#male
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s14.iff',
		'object/tangible/wearables/jacket/shared_jacket_s16.iff',
		'object/tangible/wearables/pants/shared_pants_s14.iff',
		'object/tangible/wearables/boots/shared_boots_s05.iff'
		]
def GetMarksmanItems(itemList, gender):
	if gender == 'female':
		#female
		itemList += [
		'object/tangible/wearables/shirt/shared_shirt_s24.iff',
		'object/tangible/wearables/pants/shared_pants_s24.iff',
		'object/tangible/wearables/vest/shared_vest_s10.iff',
		'object/tangible/wearables/shoes/shared_shoes_s02.iff'
		]
	else:
		#male	
		itemList += [
		'object/tangible/wearables/jacket/shared_jacket_s05.iff',
		'object/tangible/wearables/pants/shared_pants_s04.iff',
		'object/tangible/wearables/boots/shared_boots_s21.iff'
		]
	
def GetWookieeItems(itemList, profession, gender):
	if profession == 'science_medic':
		itemList.append('object/tangible/wearables/jacket/shared_jacket_s24.iff')

#Ithorian
def GetIthorianItems(itemList, profession, gender):
	itemList += IthorianItems[gender + ':' + profession]
	
ProfessionItems = {
	'social_entertainer' : ['object/tangible/instrument/shared_slitherhorn.iff'],
	'science_medic' : ['object/tangible/crafting/station/shared_food_tool.iff'],
	'outdoors_scout' : ['object/tangible/crafting/station/shared_generic_tool.iff', 'object/tangible/wearables/shoes/shared_shoes_s02.iff'],
	'crafting_artisan' : ['object/tangible/crafting/station/shared_generic_tool.iff','object/tangible/survey_tool/shared_survey_tool_mineral.iff']	
}
	
IthorianItems = {
	'female:social_entertainer' : ['object/tangible/wearables/ithorian/shared_ith_pants_s11.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s09.iff'],
	'male:social_entertainer' : ['object/tangible/wearables/ithorian/shared_ith_pants_s08.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s01.iff'],
	'female:outdoors_scout' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s04.iff','object/tangible/wearables/ithorian/shared_ith_pants_s21.iff'],
	'male:outdoors_scout' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s04.iff','object/tangible/wearables/ithorian/shared_ith_pants_s21.iff'],
	'female:science_medic' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s01.iff','object/tangible/wearables/ithorian/shared_ith_pants_s03.iff'],
	'male:science_medic' : ['object/tangible/wearables/ithorian/shared_ith_jacket_s02.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s12.iff'],
	'female:crafting_artisan' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s06.iff', 'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff'],
	'male:crafting_artisan' : ['object/tangible/wearables/ithorian/shared_ith_vest_s02.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s10.iff','object/tangible/wearables/ithorian/shared_ith_pants_s06.iff'],
	'female:combat_brawler' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s02.iff','object/tangible/wearables/ithorian/shared_ith_pants_s07.iff', 'object/tangible/wearables/ithorian/shared_ith_gloves_s01.iff'],
	'male:combat_brawler' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s02.iff','object/tangible/wearables/ithorian/shared_ith_jacket_s16.iff','object/tangible/wearables/ithorian/shared_ith_pants_s04.iff'],
	'female:combat_marksman' : ['object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s06.iff', 'object/tangible/wearables/ithorian/shared_ith_vest_s02.iff'],
	'male:combat_brawler' : ['object/tangible/wearables/ithorian/shared_ith_jacket_s05.iff','object/tangible/wearables/ithorian/shared_ith_shirt_s05.iff','object/tangible/wearables/ithorian/shared_ith_pants_s04.iff']
}
#All Get These
CommonItems = [
	'object/tangible/food/shared_fruit_melon.iff',
	'object/weapon/ranged/pistol/shared_pistol_cdef_noob.iff',
	'object/weapon/melee/knife/shared_knife_survival.iff'
]