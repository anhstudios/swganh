/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_PersistAttribute`;
DELIMITER //
CREATE PROCEDURE `sp_PersistAttribute`(IN `in_object_id` BIGINT, IN `in_attribute_name` VARCHAR(255), IN `in_attribute_value` VARCHAR(255))
BEGIN
DECLARE in_attribute_id INT;
DECLARE id_exists BIGINT;
select id from swganh_static.attributes sa where sa.name = in_attribute_name limit 1 into in_attribute_id ;
select object_id from object_attributes where object_id = in_object_id and attribute_id = in_attribute_id limit 1 into id_exists;
if id_exists <> 0 then
	update object_attributes set attribute_value = in_attribute_value where object_id = in_object_id and attribute_id = in_attribute_id;
else
	insert into object_attributes (object_id, attribute_id, attribute_value) values (in_object_id, in_attribute_id, in_attribute_value);
end if;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
