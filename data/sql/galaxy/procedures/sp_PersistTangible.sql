-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.24-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win32
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-08-12 16:17:22
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for procedure galaxy.sp_PersistTangible
DROP PROCEDURE IF EXISTS `sp_PersistTangible`;
DELIMITER //
CREATE DEFINER=`swganh`@`localhost` PROCEDURE `sp_PersistTangible`(IN `in_object_id` BIGINT, IN `in_scene_id` INT, IN `in_parent_id` BIGINT, IN `template_string` VARCHAR(255), IN `posX` dOUBLE, IN `posY` dOUBLE, IN `posZ` DOUBLE, IN `oX` DOUBLE, IN `oY` DOUBLE, IN `oZ` DOUBLE, IN `oW` DOUBLE, IN `in_complexity` DOUBLE, IN `in_stf_name_file` VARCHAR(255), IN `in_stf_name_string` VARCHAR(255), IN `in_custom_name` VARCHAR(50), IN `in_volume` INT, IN `in_arrangement_id` INT, IN `in_permission_type` INT, IN `in_customization` BLOB, IN `in_options_bitmask` INT, IN `in_incaptimer` INT, IN `in_condition_damage` INT, IN `in_max_condition` INT, IN `in_is_static` INT)
BEGIN

    CALL sp_PersistObject(in_object_id, in_scene_id, in_parent_id, template_string, posX, posY, posZ, oX, oY, oZ, oW, in_complexity, in_stf_name_file,
        in_stf_name_string, in_custom_name, in_volume, in_arrangement_id, in_permission_type);
-- now update tangible
    update tangible set customization = in_customization, options_bitmask = in_options_bitmask, incap_timer = in_incaptimer,
        condition_damage = in_condition_damage, max_condition = in_max_condition, is_static = in_is_static
    where tangible.id = in_object_id;

END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
