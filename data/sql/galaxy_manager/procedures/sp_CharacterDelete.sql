# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-14 15:50:19
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy_manager.sp_CharacterDelete
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CharacterDelete`(IN character_id BIGINT, IN account_id BIGINT)
BEGIN
	
	
	

	DECLARE check_value INT(11);
    DECLARE deleted_code INT(11);
    DECLARE acc_id BIGINT;

	
	
	

	SET deleted_code = 0;
    SELECT C.referenceId from characters A
    INNER JOIN players_characters B on (A.id = B.character_id)
    INNER JOIN player C on (B.player_id = C.id) 
    WHERE A.entity_id = character_id INTO acc_id;
    
    IF account_id = acc_id THEN

        UPDATE characters SET archived = 1 WHERE entity_id = character_id;

        UPDATE characters SET deletedAt = (NOW()) WHERE entity_id = character_id;
    
    END IF;
	
	
	

	SELECT COUNT(*) from characters WHERE entity_id = character_id AND archived = 1 INTO check_value;

	IF check_value > 0 THEN SET deleted_code = 1;
	END IF;

	SELECT deleted_code;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
