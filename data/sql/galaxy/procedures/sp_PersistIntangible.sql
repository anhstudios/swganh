
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_PersistIntangible
DROP PROCEDURE IF EXISTS `sp_PersistIntangible`;
DELIMITER //
CREATE PROCEDURE `sp_PersistIntangible`(IN `object_id` BIGINT, IN `file` VARCHAR(50), IN `string` VARCHAR(50), IN in_generic_int INT)
BEGIN
REPLACE INTO intangible set id = object_id, stf_detail_file = file, stf_detail_string = string, generic_int = in_generic_int;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
