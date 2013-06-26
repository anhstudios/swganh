/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for procedure galaxy.sp_PersistObject
DROP PROCEDURE IF EXISTS `sp_PersistObject`;
DELIMITER //
CREATE PROCEDURE `sp_PersistObject`(IN `in_object_id` BIGINT, IN `in_scene_id` INT, IN `in_parent_id` BIGINT, IN `template_string` VARCHAR(255), IN `posX` dOUBLE, IN `posY` dOUBLE, IN `posZ` DOUBLE, IN `oX` DOUBLE, IN `oY` DOUBLE, IN `oZ` DOUBLE, IN `oW` DOUBLE, IN `in_complexity` DOUBLE, IN `in_stf_name_file` VARCHAR(255), IN `in_stf_name_string` VARCHAR(255), IN `in_custom_name` VARCHAR(50), IN `in_volume` INT, IN `in_arrangement_id` INT, IN `in_permission_type` INT, IN `in_type_id` INT)
BEGIN
-- First get our template_id
    DECLARE iff_template INT;
    DECLARE does_exist INT;
    
     select count(*) from object o where o.id = in_object_id into does_exist;
	 SELECT swganh_static.iff_templates.id FROM swganh_static.iff_templates WHERE swganh_static.iff_templates.iff_template LIKE template_string INTO iff_template;
    IF does_exist > 0 THEN
      -- Now update our values
    	UPDATE object SET scene_id = in_scene_id, parent_id = in_parent_id, iff_template_id = iff_template, 
        x_position = posX, y_position = posY, z_position = posZ, x_orientation = oX, y_orientation = oY, z_orientation = oZ,
        w_orientation = oW, complexity = in_complexity, stf_name_file = in_stf_name_file, stf_name_string= in_stf_name_string,
        custom_name = in_custom_name, volume = in_volume, arrangement_id = in_arrangement_id, permission_type = in_permission_type, type_id = in_type_id, updated_at = NOW()
    	WHERE id = in_object_id;
    ELSE
    	INSERT INTO object (id, scene_id,parent_id, iff_template_id, x_position, y_position, z_position, x_orientation, y_orientation, z_orientation, w_orientation,
    			complexity, stf_name_file, stf_name_string, custom_name, volume, arrangement_id, permission_type, type_id, created_at, updated_at)
    			values (in_object_id, in_scene_id,in_parent_id, iff_template, posX, posY, posZ, oX, oY, oZ, oW,
    				in_complexity, in_stf_name_file, in_stf_name_string, in_custom_name, in_volume, in_arrangement_id, in_permission_type, in_type_id, NOW(), NOW());
    END IF;
END //
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
