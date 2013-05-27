/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_LoadChatData`;

DELIMITER //

CREATE PROCEDURE `sp_LoadChatData`()
BEGIN

	SELECT c.room_id, c.is_private, c.is_muted, c.name, c.owner, o.custom_name, c.creator, o2.custom_name, c.title FROM `chat_rooms` c
	LEFT JOIN `object` o ON o.id = c.owner
	LEFT JOIN `object` o2 ON o2.id = c.creator;
	
	SELECT * FROM `chat_room_bans`;
	
	SELECT cm.room_id, cm.moderator_id FROM `chat_moderators` cm
	LEFT JOIN `object` o ON o.id = cm.moderator_id;

END //
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;