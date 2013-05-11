/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_GetAttributes`;
DELIMITER //
CREATE PROCEDURE `sp_GetAttributes`(IN `in_object_id` BIGINT)
BEGIN
	select object_attributes.attribute_value, swganh_static.attributes.name
	from object_attributes
	left join swganh_static.attributes on (swganh_static.attributes.id = object_attributes.attribute_id)
	where object_attributes.object_id = in_object_id;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
