DELIMITER $$

DROP PROCEDURE IF EXISTS `sp_GetStaticObjects` $$
CREATE PROCEDURE `sp_GetStaticObjects`(IN `parent_id` BIGINT, IN `planet_id` INT)
BEGIN
    
	/*Load Static Buildings*/
	SELECT buildings.id,buildings.oX,buildings.oY,buildings.oZ,buildings.oW,buildings.x,
		buildings.y,buildings.z,building_types.model,building_types.width,building_types.length,
		building_types.file,building_types.name,building_types.family_id
		FROM buildings INNER JOIN building_types ON buildings.type_id = building_types.id
		WHERE buildings.planet_id = planet_id;

	SELECT cells.id, cells.building_id
		FROM cells
		INNER JOIN buildings ON cells.building_id = buildings.id
		WHERE buildings.planet_id = planet_id;

	SELECT spawn_clone.parentId,spawn_clone.oX,spawn_clone.oY,spawn_clone.oZ,spawn_clone.oW,
		spawn_clone.cell_x,spawn_clone.cell_y,spawn_clone.cell_z,spawn_clone.city
		FROM  spawn_clone
		INNER JOIN cells ON spawn_clone.parentid = cells.id
		INNER JOIN buildings ON cells.building_id = buildings.id
		WHERE buildings.planet_id = planet_id;

  /* Load Terminals */
	SELECT terminals.id, terminals.parent_id, terminals.oX, terminals.oY, terminals.oZ,terminals.oW,terminals.x,
		terminals.y,terminals.z,terminals.terminal_type,terminal_types.object_string,terminal_types.name,terminal_types.file,
		terminals.dataStr,terminals.dataInt1,terminals.customName
		FROM terminals INNER JOIN terminal_types ON (terminals.terminal_type = terminal_types.id)
		WHERE (terminal_types.name NOT LIKE 'unknown') AND (terminals.planet_id = planet_id);

  /* Load Elevator Data */
	SELECT ted.* FROM terminal_elevator_data ted INNER JOIN terminals t ON t.id = ted.id WHERE t.planet_id = planet_id;

	SELECT containers.id FROM containers INNER JOIN container_types ON (containers.container_type = container_types.id)
		WHERE (container_types.name NOT LIKE 'unknown') AND (containers.parent_id = parent_id) AND (containers.planet_id = planet_id);


  /* Ticket collectors */
  SELECT * FROM ticket_collectors tc WHERE tc.planet_id = planet_id;


  	/*Load Static Persistent NPCS*/
	SELECT persistent_npcs.id,persistent_npcs.parentId,persistent_npcs.firstName,persistent_npcs.lastName,persistent_npcs.posture,persistent_npcs.state,persistent_npcs.cl,
	persistent_npcs.oX,persistent_npcs.oY,persistent_npcs.oZ,persistent_npcs.oW,persistent_npcs.x,persistent_npcs.y,persistent_npcs.z,
	persistent_npcs.type,persistent_npcs.stf_variable_id,persistent_npcs.stf_file_id,faction.name,
	persistent_npcs.moodId,persistent_npcs.family,persistent_npcs.scale
	FROM persistent_npcs
	INNER JOIN faction ON (persistent_npcs.faction = faction.id)
	WHERE (persistent_npcs.planet_id = planet_id);

	/*Load Static Shuttles */
	SELECT shuttles.id,shuttles.parentId,shuttles.firstName,shuttles.lastName,
  shuttles.oX,shuttles.oY,shuttles.oZ,shuttles.oW,shuttles.x,shuttles.y,shuttles.z,
  shuttle_types.object_string,shuttle_types.name,shuttle_types.file,
  shuttles.awayTime,shuttles.inPortTime,shuttles.collectorId
  FROM shuttles
  INNER JOIN shuttle_types ON (shuttles.shuttle_type = shuttle_types.id)
  WHERE (shuttles.planet_id = planet_id);


END $$

DELIMITER ;