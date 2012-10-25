##   scene_id, location_name, x,y,z
class StartingLocation:
	__slots__ = ('scene_id', 'x', 'y', 'z')
	
	def __init__(self, scene_id, x, y, z):
		self.scene_id = scene_id
		self.x = x
		self.y = y
		self.z = z
	
Locations = { 
	'coronet'         :StartingLocation(1,-137,0,-4723),
	'tyrena'          :StartingLocation(1,-5045,0,-2294),
	'kor_vella'       :StartingLocation(1,-3138,0,2808),
	'doaba_guerfel'   :StartingLocation(1,33360,5525),
	'theed'           :StartingLocation(6,-4856,0,4162),
	'moenia'          :StartingLocation(6,4800,0,-4700),
	'keren'           :StartingLocation(6,1441,0,2771),
	'kaadara'         :StartingLocation(6,5209,0,6677),
	'narmle'          :StartingLocation(7,-5310,0,-2221),
	'restuss'         :StartingLocation(7,5362,0,5663),
	'dearic'          :StartingLocation(8,335,0,-2931),
	'nashal'          :StartingLocation(8,4371,0,5165),
	'bestine'         :StartingLocation(9,-1290,0,-3590),
	'mos_espa'        :StartingLocation(9,-2902,0,2130),
	'mos_eisley'      :StartingLocation(9,35280,-4804),
	'mos_entha'       :StartingLocation(9,12910,3138),
	'tutorial'        :StartingLocation(42,0,0,0),
	'default_location':StartingLocation(42,27.4,-42,-161.10)
}