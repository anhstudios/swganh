DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetPlanetaryTravelRoutes` $$
CREATE PROCEDURE `sp_GetPlanetaryTravelRoutes`()
BEGIN
	SELECT srcId + 1 AS srcId, destId + 1 AS destId, price FROM `travel_planet_routes`;
END $$

DELIMITER ;