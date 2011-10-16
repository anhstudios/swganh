# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 13:15:16
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for function galaxy_manager.sf_CharacterNameCreate
DELIMITER //
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_CharacterNameCreate`(base_model_string CHAR(64)) RETURNS char(64) CHARSET utf8
BEGIN
	DECLARE shortSpecies CHAR(16);
	DECLARE gender INT(11);
	DECLARE gen_firstname CHAR(16);
	DECLARE gen_lastname CHAR(16);
	DECLARE gen_fullname CHAR(64);

	IF base_model_string like '%female%' then SET gender = 0;
		ELSE SET gender = 1;
	END IF;

    SELECT sf_SpeciesShort(base_model_string) INTO shortSpecies;

    SELECT sf_GenerateFirstName(shortSpecies, gender) INTO gen_firstname;
    SELECT sf_GenerateLastName(shortSpecies, gender) INTO gen_lastname;

    IF shortSpecies = 'wookiee' THEN SET gen_lastname = NULL;
    END IF;

    IF gen_lastname IS NOT NULL THEN SELECT CONCAT_WS(' ', gen_firstname, gen_lastname) INTO gen_fullname;
    ELSE SET gen_fullname = gen_firstname;
    END IF;

	RETURN gen_fullname;

	END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
