# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:38:43
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for function galaxy_manager.sf_GenerateFirstName
DELIMITER //
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_GenerateFirstName`(start_species CHAR(16), start_gender INT(11)) RETURNS char(16) CHARSET utf8
BEGIN
    DECLARE gen_firstname CHAR(16);
    DECLARE raceId INT(8);
      
    SELECT id FROM species WHERE species_name = start_species INTO raceId;

    SELECT firstname FROM namegen_firstname WHERE species = raceId AND gender = start_gender ORDER BY RAND() LIMIT 1 INTO gen_firstname;

    RETURN gen_firstname;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
