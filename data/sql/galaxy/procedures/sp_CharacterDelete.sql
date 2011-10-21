# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-17 22:07:04
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

use galaxy;

# Dumping structure for procedure galaxy.sp_CharacterDelete
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CharacterDelete`(IN `character_id` BIGINT, IN `account_id` BIGINT)
BEGIN
	 DECLARE check_value INT(11);
    DECLARE deleted_code INT(11);
    DECLARE acc_id BIGINT;

	 SET deleted_code = 0;
	 SELECT D.reference_id
	 FROM object A
    INNER JOIN player_accounts_players C ON (A.id = C.player_character_id)
    INNER JOIN player_account D ON (C.player_id = D.id)
    WHERE A.id = character_id INTO acc_id;
    
    IF account_id = acc_id THEN
        UPDATE object SET deleted_at = NOW() 
		  WHERE object.id = character_id;
    END IF;
	
	SELECT COUNT(*) FROM object A 
	WHERE A.id = character_id AND A.deleted_at <= NOW()
	INTO check_value;

	IF check_value > 0 THEN SET deleted_code = 1;
	END IF;

	SELECT deleted_code;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
