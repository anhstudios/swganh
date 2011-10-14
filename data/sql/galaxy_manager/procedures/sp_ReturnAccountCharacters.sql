# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-14 14:48:39
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy_manager_entity.sp_ReturnAccountCharacters
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_ReturnAccountCharacters`(IN account_id INT)
BEGIN
    SELECT A.entity_id, A.firstName, A.lastName, A.jediState, D.baseModel
    FROM characters A
    INNER JOIN player B ON (account_id = B.referenceId)
    INNER JOIN players_characters C ON (B.id = C.player_id)
    INNER JOIN appearance D ON (A.entity_id = D.entity_id)
    WHERE A.id = C.character_id and A.archived = 0;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
