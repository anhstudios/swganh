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

# Dumping structure for function galaxy_manager.sf_CharacterNameProfaneCheck
DELIMITER //
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_CharacterNameProfaneCheck`(start_firstname CHAR(32)) RETURNS char(32) CHARSET utf8
BEGIN
	DECLARE check_name char(32);
	DECLARE check_value INT(11);
	DECLARE error_code char(32);

	SELECT COUNT(*) from name_profane where name LIKE start_firstname INTO check_value;

	IF check_value > 0 THEN SET error_code = 'name_declined_profane';
	END IF;

	RETURN error_code;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
