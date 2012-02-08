
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_GetPlayer`;

DELIMITER //
CREATE PROCEDURE `sp_GetPlayer`(IN `object_id` BIGINT)
BEGIN
    CALL sp_GetObject(object_id);

    SELECT * 
    FROM player
    WHERE player.id = object_id ;

    SELECT status_flag.flag
    FROM status_flag
    LEFT JOIN players_status_flags ON players_status_flags.status_flag_id = status_flag.id
    WHERE players_status_flags.player_id = object_id;

    SELECT profile_flag.flag
    FROM profile_flag
    LEFT JOIN players_profile_flags ON players_profile_flags.profile_flag_id = profile_flag.id
    WHERE players_profile_flags.player_id = object_id;

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
