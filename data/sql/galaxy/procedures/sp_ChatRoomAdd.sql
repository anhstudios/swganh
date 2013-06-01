/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_ChatRoomAdd`;

DELIMITER //

CREATE PROCEDURE `sp_ChatRoomAdd`(IN in_is_private TINYINT(1), IN in_is_muted TINYINT(1), 
	IN in_name CHAR(50), IN in_creator_id BIGINT(20), IN in_title CHAR(50))
BEGIN

	DECLARE max_id INT;

	INSERT INTO `chat_rooms` 
	(`is_private`, `is_muted`, `name`, `creator`, `owner`, `title`) 
	VALUES (in_is_private, in_is_muted, in_name, in_creator_id, in_creator_id, in_title);
	
	SELECT MAX(room_id) FROM chat_rooms INTO max_id;
	
	INSERT INTO `chat_moderators`
	(`room_id`, `moderator_id`)
	VALUES (max_id, in_creator_id);
	
	SELECT max_id;

END //
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;