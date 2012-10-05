-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.27-MariaDB-log - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-04 17:17:01
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for procedure galaxy.sp_PersistAttribute
DROP PROCEDURE IF EXISTS `sp_PersistAttribute`;
DELIMITER //
CREATE DEFINER=`swganh`@`localhost` PROCEDURE `sp_PersistAttribute`(IN `in_object_id` BIGINT, IN `in_attribute_name` VARCHAR(255), IN `in_attribute_value` VARCHAR(255))
BEGIN
DECLARE in_attribute_id INT;
DECLARE id_exists INT;
select id from swganh_static.attributes sa where sa.name = in_attribute_name into in_attribute_id;
select id from object_attributes where object_id = in_object_id and attribute_id = in_attribute_id into id_exists;
if id_exists <> 0 then
	update object_attributes set object_id = in_object_id, attribute_id = in_attribute_id, attribute_value = in_attribute_value;
else
	insert into object_attributes (object_id, attribute_id, attribute_value) values (in_object_id, in_attribute_id, in_attribute_value);
end if;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
