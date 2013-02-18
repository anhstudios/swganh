DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetTravelPoints` $$
CREATE PROCEDURE `sp_GetTravelPoints`()
BEGIN
	SELECT DISTINCT(terminals.dataStr) AS descriptor, terminals.x, terminals.y, terminals.z, terminals.dataInt1 AS port_type, terminals.dataInt3 AS taxes, 
		terminals.planet_id + 1 AS scene_id, spawn_shuttle.x AS spawn_x, spawn_shuttle.y AS spawn_y, spawn_shuttle.z AS spawn_z
		FROM `terminals`
		INNER JOIN spawn_shuttle ON (terminals.dataInt3 = spawn_shuttle.id)
		WHERE terminals.terminal_type = 16 AND terminals.parent_id = 0
		GROUP BY terminals.dataStr;

	SELECT DISTINCT(terminals.dataStr) AS descriptor, buildings.x, buildings.y, buildings.z, terminals.dataInt1 AS port_type, terminals.dataInt3 AS taxes, 
		terminals.planet_id + 1 AS scene_id, spawn_shuttle.x AS spawn_x, spawn_shuttle.y AS spawn_y, spawn_shuttle.z AS spawn_z
		FROM `terminals`
		INNER JOIN spawn_shuttle ON (terminals.dataInt3 = spawn_shuttle.id)
		INNER JOIN cells ON (terminals.parent_id = cells.id)
		INNER JOIN buildings ON (cells.building_id = buildings.id)
		WHERE terminals.terminal_type = 16 AND terminals.parent_id <> 0
		GROUP BY terminals.dataStr;
END $$

DELIMITER ;