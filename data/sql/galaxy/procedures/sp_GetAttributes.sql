-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.27-MariaDB-log - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-04 14:15:28
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for procedure galaxy.sp_GetAttributes
DROP PROCEDURE IF EXISTS `sp_GetAttributes`;
DELIMITER //
CREATE DEFINER=`swganh`@`localhost` PROCEDURE `sp_GetAttributes`(IN `iff_template_id` INT)
BEGIN
	select swganh_static.object_attribute_defaults.attribute_value, swganh_static.attributes.name
	from swganh_static.object_attribute_defaults
	inner join swganh_static.attributes on (swganh_static.attributes.id = swganh_static.object_attribute_defaults.attribute_id)
	where swganh_static.object_attribute_defaults.id = iff_template_id;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
