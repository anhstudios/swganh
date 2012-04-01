
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_UpdateFriends
DROP PROCEDURE IF EXISTS `sp_UpdateFriends`;
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_UpdateFriends`(IN `object_id` BIGINT, IN `friend_id_` BIGINT)
BEGIN
IF NOT EXISTS(SELECT id from friend_list where player_id = object_id and friend_id = friend_id_) THEN
	INSERT INTO friend_list SET player_id = object_id, friend_id = friend_id_;
END IF;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
