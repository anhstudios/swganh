/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_GetAttributes`;
DELIMITER //
CREATE PROCEDURE `sp_GetAttributes`(IN `iff_template` VARCHAR(255), IN `in_object_id` BIGINT)
BEGIN
	DECLARE iff_template_id INT;
	SELECT swganh_static.iff_templates.id FROM swganh_static.iff_templates WHERE swganh_static.iff_templates.iff_template = iff_template INTO iff_template_id;
	(select object_attributes.attribute_value, swganh_static.attributes.name
		from object_attributes
		left join swganh_static.attributes on (swganh_static.attributes.id = object_attributes.attribute_id)
		where object_attributes.id = iff_template_id)
	UNION
(select swganh_static.object_attribute_defaults.attribute_value, swganh_static.attributes.name
	from swganh_static.object_attribute_defaults
	left join swganh_static.attributes on (swganh_static.attributes.id = swganh_static.object_attribute_defaults.attribute_id)
	where swganh_static.object_attribute_defaults.id = iff_template_id);
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;