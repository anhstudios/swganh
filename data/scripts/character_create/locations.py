##   scene_id, location_name, x,y,z
class StartingLocation:
	__slots__ = ('name', 'x', 'y', 'z')
	
	def __init__(self, name, x, y, z):
		self.name = name
		self.x = x
		self.y = y
		self.z = z
	
Locations = { 
	'coronet'         :StartingLocation('corellia',-137,0,-4723),
	'tyrena'          :StartingLocation('corellia',-5045,0,-2294),
	'kor_vella'       :StartingLocation('corellia',-3138,0,2808),
	'doaba_guerfel'   :StartingLocation('corellia',3336,0,5525),
	'theed'           :StartingLocation('naboo',-4856,0,4162),
	'moenia'          :StartingLocation('naboo',4800,0,-4700),
	'keren'           :StartingLocation('naboo',1441,0,2771),
	'kaadara'         :StartingLocation('naboo',5209,0,6677),
	'narmle'          :StartingLocation('rori',-5310,0,-2221),
	'restuss'         :StartingLocation('rori',5362,0,5663),
	'dearic'          :StartingLocation('talus',335,0,-2931),
	'nashal'          :StartingLocation('talus',4371,0,5165),
	'bestine'         :StartingLocation('tatooine',-1290,0,-3590),
	'mos_espa'        :StartingLocation('tatooine',-2902,0,2130),
	'mos_eisley'      :StartingLocation('tatooine',3528,0,-4804),
	'mos_entha'       :StartingLocation('tatooine',1291,0,3138),
	'tutorial'        :StartingLocation('tutorial',0,0,0),
	'default_location':StartingLocation('tutorial',27.4,-42,-161.10)
}