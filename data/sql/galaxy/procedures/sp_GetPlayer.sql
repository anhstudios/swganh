# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-14 14:44:33
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_GetPlayer
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetPlayer`(IN `object_id` BIGINT)
BEGIN
CALL sp_GetObject(object_id);
select * from player p, players_status_flags ps, players_profile_flags pp where p.id = object_id and ps.player_id = object_id and pp.player_id = object_id;
call sp_GetPlayerAbilities(object_id);
call sp_GetPlayerDraftSchematics(object_id);
call sp_GetFriends(object_id);
call sp_GetFSQuestList(object_id);
call sp_GetIgnoredList(object_id);
call sp_GetQuestJournal(object_id);
call sp_GetWaypoint(object_id);
call sp_GetExperienceList(object_id);
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
