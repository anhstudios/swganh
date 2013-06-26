/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_ChatTransferRoomOwner`;

DELIMITER //

CREATE PROCEDURE `sp_ChatPersistRoom`(IN in_room_id INT, 
	IN in_is_private TINYINT(1), IN in_is_muted TINYINT(1), IN in_name CHAR(50), 
	IN in_creator_id BIGINT(20), IN in_owner_id BIGINT(20), IN in_title CHAR(50))
BEGIN

	UPDATE `chat_rooms` 
	SET `is_private` = in_is_private, `is_muted` = in_is_muted, `name` = in_name,
	`creator_id` = in_creator_id, `owner_id` = in_owner_id, `title` = in_title
	WHERE `room_id` = in_room_id;

END //
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;