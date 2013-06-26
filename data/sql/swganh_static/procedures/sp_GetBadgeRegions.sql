DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetBadgeRegions` $$
CREATE PROCEDURE `sp_GetBadgeRegions`()
BEGIN

	SELECT b.id, ba.name, b.planet_id, b.parent_id, r.region_name, r.region_file, r.x, r.z, r.width,
		r.length FROM badge_regions b 
		INNER JOIN planet_regions r ON r.id = b.region_id
		INNER JOIN badges ba ON ba.id = b.badge_id;

END $$

DELIMITER ;