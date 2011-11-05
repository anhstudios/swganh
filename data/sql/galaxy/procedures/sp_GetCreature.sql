# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-26 22:53:03
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_GetCreature
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetCreature`(IN `object_id` BIGINT)
BEGIN
    call sp_GetTangible(object_id);

    SELECT creature.*, mood.name as mood_animation 
    FROM creature 
    LEFT JOIN iff_templates ON (creature.disguise_template_id = iff_templates.id)
    LEFT JOIN mood ON (creature.mood_id = mood.id)
    WHERE creature.id = object_id;

    call sp_GetCreatureSkills(object_id);
    call sp_GetCreatureSkillMods(object_id);
    call sp_GetContainedObjects(object_id);
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
