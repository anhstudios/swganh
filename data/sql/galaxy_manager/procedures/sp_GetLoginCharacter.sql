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

# Dumping structure for procedure galaxy_manager_entity.sp_GetLoginCharacter
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetLoginCharacter`(IN id BIGINT(20),IN account_id BIGINT(20))
BEGIN

	SELECT A.entity_id, B.baseModel, B.gender, B.species, C.x, C.y, C.z,

    C.oX, C.oY, C.oZ, C.oW, D.terrainMap

	FROM characters A

	INNER JOIN appearance B ON (A.entity_id = B.entity_id)

    INNER JOIN transform C on (A.entity_id = C.entity_id)

    INNER JOIN planet D on (C.planet_id = D.planet_id)

    INNER JOIN players_characters E on (A.id = E.character_id)

    INNER JOIN player F on (E.player_id = F.id)

    WHERE A.entity_id = id and A.archived = 0 and F.referenceId = account_id;

END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
