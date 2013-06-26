/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_RemoveLocation
DROP PROCEDURE IF EXISTS `sp_RemoveLocation`;
DELIMITER //
CREATE PROCEDURE `sp_RemoveLocation`(IN `location_id_` INT(10), IN `planet_id_` INT(11), IN `name_` TEXT)
BEGIN
IF EXISTS(SELECT id FROM planetmap WHERE id = location_id_ AND planet_id = planet_id_ AND `name` = name_) THEN
	DELETE FROM planetmap WHERE id = location_id_ AND planet_id = planet_id_ AND `name` = name_;
END IF;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
