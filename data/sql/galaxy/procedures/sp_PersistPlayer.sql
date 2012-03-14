
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_PersistPlayer`;

DELIMITER //
CREATE PROCEDURE `sp_PersistPlayer`(
    IN `in_object_id` BIGINT, 
    IN `in_scene_id` INT, 
    IN `in_parent_id` BIGINT, 
    IN `template_string` VARCHAR(255), 
    IN `posX` DOUBLE, 
    IN `posY` DOUBLE, 
    IN `posZ` DOUBLE, 
    IN `oX` DOUBLE, 
    IN `oY` DOUBLE, 
    IN `oZ` DOUBLE, 
    IN `oW` DOUBLE, 
    IN `in_complexity` DOUBLE, 
    IN `in_stf_name_file` VARCHAR(255), 
    IN `in_stf_name_string` VARCHAR(255), 
    IN `in_custom_name` VARCHAR(50), 
    IN `in_volume` INT, 
    IN `in_profession_tag` VARCHAR(255), 
    IN `in_total_playtime` BIGINT, 
    IN `in_csr_tag` SMALLINT, 
    IN `in_max_force` INT, 
    IN `in_experimentation_enabled` TINYINT, 
    IN `in_crafting_stage` INT, 
    IN `in_nearest_crafting_station` BIGINT, 
    IN `in_experimentation_points` INT, 
    IN `in_accomplishment_counter` INT, 
    IN `in_current_language` INT, 
    IN `in_current_stomach` INT, 
    IN `in_max_stomach` INT, 
    IN `in_current_drink` INT, 
    IN `in_max_drink` INT, 
    IN `in_jedi_state` INT)
BEGIN
    CALL sp_PersistObject(in_object_id, in_scene_id, in_parent_id, template_string, posX, posY, posZ, oX, oY, oZ, oW, in_complexity, in_stf_name_file,
        in_stf_name_string, in_custom_name, in_volume);

    update player set profession_tag = in_profession_tag, total_playtime = in_total_playtime, csr_tag = in_csr_tag, max_force = in_max_force,
        experimentation_enabled = in_experimentation_enabled, crafting_stage = in_crafting_stage, nearest_crafting_station = in_nearest_crafting_station,
        accomplishment_counter = in_accomplishment_counter, current_language = in_current_language, current_stomach = in_current_stomach, max_stomach = in_max_stomach,
        current_drink = in_current_drink, max_drink = in_max_drink, jedi_state = in_jedi_state
    where player.id = in_object_id;
END//
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
