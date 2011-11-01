# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-11-01 16:49:14
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_PersistTangible
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_PersistTangible`(IN `in_object_id` BIGINT, IN `in_scene_id` INT, IN `in_parent_id` BIGINT,
 IN `template_string` VARCHAR(255), IN `posX` dOUBLE, IN `posY` dOUBLE, IN `posZ` DOUBLE, IN `oX` DOUBLE, IN `oY` DOUBLE, IN `oZ` DOUBLE,
 IN `oW` DOUBLE, IN `in_complexity` DOUBLE, IN `in_stf_name_file` VARCHAR(255), IN `in_stf_name_string` VARCHAR(255),
 IN `in_custom_name` VARCHAR(50), IN `in_volume` INT, IN `in_customization` BLOB, IN `in_options_bitmask` INT, IN `in_incaptimer` INT,
 IN `in_condition_damage` INT, IN `in_max_condition` INT, IN `in_is_static` INT)
BEGIN
-- First get our template_id
DECLARE iff_template INT;
SELECT iff_templates.id FROM iff_templates WHERE iff_templates.iff_template LIKE template_string INTO iff_template;
-- first update base object
CALL sp_PersistObject(in_object_id, in_scene_id, in_parent_id, iff_template, posX, posY, posZ, oX, oY, oZ, oW, in_complexity, in_stf_name_file,
in_stf_name_string, in_custom_name, in_volume);
-- now update tangible
update tangible set customization = in_customization, options_bitmask = in_options_bitmask, incap_timer = in_incaptimer,
condition_damage = in_condition_damage, max_condition = in_max_condition, is_static = in_is_static
where tangible.object_id = in_object_id;

END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
