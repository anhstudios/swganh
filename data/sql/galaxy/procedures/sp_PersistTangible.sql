/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_PersistTangible`;
DELIMITER //
CREATE PROCEDURE `sp_PersistTangible`(IN `in_object_id` BIGINT, IN `in_customization` BLOB, IN `in_options_bitmask` INT, IN `in_incaptimer` INT, IN `in_condition_damage` INT, IN `in_max_condition` INT, IN `in_is_static` INT)
BEGIN
	DECLARE does_exist INT;
    
    select count(*) from tangible t where t.id = in_object_id into does_exist;
	if does_exist > 0 then
		update tangible set customization = in_customization, options_bitmask = in_options_bitmask, incap_timer = in_incaptimer,
			condition_damage = in_condition_damage, max_condition = in_max_condition, is_static = in_is_static
		where tangible.id = in_object_id;
	else
		insert into tangible set id = in_object_id, customization = in_customization, options_bitmask = in_options_bitmask, incap_timer = in_incaptimer,
			condition_damage = in_condition_damage, max_condition = in_max_condition, is_static = in_is_static;		
	end if;
    

END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
