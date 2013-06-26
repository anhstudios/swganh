
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_GetCreature`;

DELIMITER //
CREATE PROCEDURE `sp_GetCreature`(IN `object_id` BIGINT)
BEGIN
    SELECT
        creature.*,
        mood.name as mood_animation,
        swganh_static.iff_templates.iff_template as disguise_template
    FROM creature
    LEFT JOIN swganh_static.iff_templates ON (creature.disguise_template_id = swganh_static.iff_templates.id)
    LEFT JOIN mood ON (creature.mood_id = mood.id)
    WHERE creature.id = object_id;
END//
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
