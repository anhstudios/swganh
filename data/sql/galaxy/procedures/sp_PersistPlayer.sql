/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_PersistPlayer`;
DELIMITER //
CREATE PROCEDURE `sp_PersistPlayer`(IN `in_object_id` BIGINT, IN `in_profession_tag` VARCHAR(255), IN `in_total_playtime` BIGINT, IN `in_csr_tag` SMALLINT, IN `in_max_force` INT, 
IN `in_experimentation_enabled` TINYINT, IN `in_crafting_stage` INT, IN `in_nearest_crafting_station` BIGINT, IN `in_experimentation_points` INT, IN `in_accomplishment_counter` INT, 
IN `in_current_language` INT, IN `in_current_stomach` INT, IN `in_max_stomach` INT, IN `in_current_drink` INT, IN `in_max_drink` INT, IN `in_jedi_state` INT)
BEGIN    
	DECLARE does_exist INT;
    
    select count(*) from player p where p.id = in_object_id into does_exist;
	if does_exist > 0 then
		update player set profession_tag = in_profession_tag, total_playtime = in_total_playtime, csr_tag = in_csr_tag, max_force = in_max_force,
			experimentation_enabled = in_experimentation_enabled, crafting_stage = in_crafting_stage, nearest_crafting_station = in_nearest_crafting_station,
			accomplishment_counter = in_accomplishment_counter, current_language = in_current_language, current_stomach = in_current_stomach, max_stomach = in_max_stomach,
			current_drink = in_current_drink, max_drink = in_max_drink, jedi_state = in_jedi_state
		where player.id = in_object_id;
	else
		insert into player set id = in_object_id, profession_tag = in_profession_tag, born_date = NOW(), total_playtime = in_total_playtime, csr_tag = in_csr_tag, max_force = in_max_force,
			experimentation_enabled = in_experimentation_enabled, crafting_stage = in_crafting_stage, nearest_crafting_station = in_nearest_crafting_station,
			accomplishment_counter = in_accomplishment_counter, current_language = in_current_language, current_stomach = in_current_stomach, max_stomach = in_max_stomach,
			current_drink = in_current_drink, max_drink = in_max_drink, jedi_state = in_jedi_state;		
	end if;
	
    
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
