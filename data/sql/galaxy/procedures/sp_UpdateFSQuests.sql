/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_UpdateFSQuests
DROP PROCEDURE IF EXISTS `sp_UpdateFSQuests`;
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_UpdateFSQuests`(IN `player_id_` BIGINT, IN `current_quest_mask` INT, IN `completed_quest_mask` INT)
BEGIN
DECLARE fs_completed INT;
DECLARE fs_not_completed INT;
SELECT id FROM force_sensitive_quest_list WHERE player_id = player_id AND completed = 1 INTO fs_completed;
SELECT id FROM force_sensitive_quest_list WHERE player_id = player_id AND completed = 0 INTO fs_not_completed;
REPLACE INTO force_sensitive_quest_list SET id = fs_not_completed, player_id = player_id, quest_mask = current_quest_mask, completed = 0;
REPLACE INTO force_sensitive_quest_list SET id = fs_completed, player_id = player_id, quest_mask = completed_quest_mask, completed = 1;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
