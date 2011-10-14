# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.5.16
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-08 00:17:14
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_GetExperienceList
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetExperienceList`(IN `object_id` BIGINT)
BEGIN
select t.name, x.value from xp_list x, xp_type t where x.player_id = object_id AND t.id = x.xp_type_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetFriends
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetFriends`(IN `object_id` BIGINT)
BEGIN
select o.custom_name from friend_list f, object o where f.player_id = object_id and f.friend_id = o.id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetFSQuestList
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetFSQuestList`(IN `object_id` BIGINT)
BEGIN
select fs.quest_mask, fs.completed from force_sensative_quest_list fs where fs.player_id = object_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetIgnoredList
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetIgnoredList`(IN `object_id` BIGINT)
BEGIN
select o.custom_name from ignore_list i, object o where i.player_id = object_id and i.ignored_player_id = o.id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetIntangible
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetIntangible`(IN `object_id` BIGINT)
BEGIN
call sp_GetObject(object_id);
select i.stf_detail_file, i.stf_detail_string from intangible i where i.id = object_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetObject
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetObject`(IN `object_id` BIGINT)
    COMMENT 'Get''s the base object that all game objects have'
BEGIN
select * from object where object.id = object_id;
END//
DELIMITER ;


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


# Dumping structure for procedure galaxy.sp_GetPlayerAbilities
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetPlayerAbilities`(IN `player_id` BIGINT)
BEGIN
select a.ability from ability_list a where a.player_id = player_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetPlayerDraftSchematics
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetPlayerDraftSchematics`(IN `player_id` BIGINT)
BEGIN
select d.id, d.schematic from draft_schematic_list d where d.player_id = player_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetQuestJournal
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetQuestJournal`(IN `object_id` BIGINT)
BEGIN
select q.quest_owner_id, q.name, q.active_step_bitmask, q.completed_step_bitmask, q.completed from quest_journal_list q where q.player_id = object_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetTangible
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetTangible`(IN `object_id` BIGINT)
BEGIN
call sp_GetObject(object_id);
select t.customization, t.options_bitmask, t.incap_timer, t.condition_damage, t.max_condition,
t.is_moveable from tangible t where t.id = object_id;
END//
DELIMITER ;


# Dumping structure for procedure galaxy.sp_GetWaypoint
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetWaypoint`(IN `object_id` BIGINT)
BEGIN
select o.id, o.x_position, o.y_position, o.z_position, o.x_orientation, o.y_orientation, o.z_orientation,
o.w_orientation, o.complexity, o.stf_name_file, o.stf_name_string, o.custom_name, o.volume, o.discr,
w.is_active, w.name, w.color from waypoint w, object o where w.player_id = object_id and o.id = w.id;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
