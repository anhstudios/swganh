DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetPlanetaryTravelRoutes` $$
CREATE PROCEDURE `sp_GetPlanetaryTravelRoutes`()
BEGIN
	SELECT srcId, destId, price FROM `travel_planet_routes`;
END $$

DELIMITER ;