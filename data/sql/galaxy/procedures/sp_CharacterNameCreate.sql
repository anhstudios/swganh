
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_CharacterNameCreate`;

DELIMITER //
CREATE PROCEDURE `sp_CharacterNameCreate`(
IN `base_model_string` CHAR(64))
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

    CALL sp_GenerateFirstName(shortSpecies, gender, gen_firstname);
    CALL sp_GenerateLastName(shortSpecies, gender, gen_lastname);

    IF shortSpecies = 'wookiee' THEN SET gen_lastname = NULL;
    END IF;

    IF gen_lastname IS NOT NULL THEN SELECT CONCAT_WS(' ', gen_firstname, gen_lastname) INTO gen_fullname;
    ELSE SET gen_fullname = gen_firstname;
    END IF;

	SELECT gen_fullname;
END//
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
