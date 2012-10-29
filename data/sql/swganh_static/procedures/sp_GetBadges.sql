DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetBadges` $$
CREATE PROCEDURE `sp_GetBadges`()
BEGIN

	SELECT b.id, b.name, b.type, s.name AS sound FROM badges b INNER JOIN sounds s ON s.id = b.sound;

END $$

DELIMITER ;