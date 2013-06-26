DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetMapLocations` $$
CREATE PROCEDURE `sp_GetMapLocations`()
BEGIN
	SELECT loc.id, loc.planet_id AS scene_id, loc.x, loc.z AS y, icon, loc.name, cat.main AS category_main, cat.sub AS category_sub
		FROM planetmap loc INNER JOIN swganh_static.planetmapcategory cat ON category_id = cat.id;
END $$

DELIMITER ;