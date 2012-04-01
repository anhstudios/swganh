/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_UpdateDraftSchematics
DROP PROCEDURE IF EXISTS `sp_UpdateDraftSchematics`;
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_UpdateDraftSchematics`(IN `player_id_` BIGINT, IN `schematic_id_` INT, IN `schematic_crc` INT)
BEGIN
DECLARE row_id BIGINT;
SELECT id from draft_schematic_list where player_id = player_id_ and schematic_id = schematic_id_ and schematic_crc = schematic_crc_ into row_id;
REPLACE INTO draft_schematic_list SET id = row_id, player_id = player_id_, schematic_id = schematic_id_, schematic_crc = schematic_crc_;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
