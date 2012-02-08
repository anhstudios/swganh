
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_PersistTangible`;

DELIMITER //
CREATE PROCEDURE `sp_PersistTangible`(IN `in_object_id` BIGINT, IN `in_scene_id` INT, IN `in_parent_id` BIGINT, IN `template_string` VARCHAR(255), IN `posX` dOUBLE, IN `posY` dOUBLE, IN `posZ` DOUBLE, IN `oX` DOUBLE, IN `oY` DOUBLE, IN `oZ` DOUBLE, IN `oW` DOUBLE, IN `in_complexity` DOUBLE, IN `in_stf_name_file` VARCHAR(255), IN `in_stf_name_string` VARCHAR(255), IN `in_custom_name` VARCHAR(50), IN `in_volume` INT, IN `in_customization` BLOB, IN `in_options_bitmask` INT, IN `in_incaptimer` INT, IN `in_condition_damage` INT, IN `in_max_condition` INT, IN `in_is_static` INT)
BEGIN

    CALL sp_PersistObject(in_object_id, in_scene_id, in_parent_id, template_string, posX, posY, posZ, oX, oY, oZ, oW, in_complexity, in_stf_name_file,
        in_stf_name_string, in_custom_name, in_volume);
-- now update tangible
    update tangible set customization = in_customization, options_bitmask = in_options_bitmask, incap_timer = in_incaptimer,
        condition_damage = in_condition_damage, max_condition = in_max_condition, is_static = in_is_static
    where tangible.id = in_object_id;

END//
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
