
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_UpdateQuestJournal
DROP PROCEDURE IF EXISTS `sp_UpdateQuestJournal`;
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_UpdateQuestJournal`(IN `player_id_` BIGINT, IN `owner_id_` BIGINT, IN `quest_crc_` INT, IN `bitmask_step` INT, IN `completed_bitmask_step` INT, IN `completed_flag` INT)
BEGIN
DECLARE found_id BIGINT(20);
select id from quest_journal_list where player_id = player_id_ and owner_id = owner_id_ and quest_crc = quest_crc_ into found_id;
REPLACE INTO quest_journal_list set id = found_id, player_id = player_id_, owner_id = owner_id_, quest_crc = quest_crc_, 
active_step_bitmask = bitmask_step, completed_step_bitmask = completed_bitmask_step, completed = completed_flag;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
