-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.5.8


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema galaxy
--

CREATE DATABASE IF NOT EXISTS galaxy;
USE galaxy;

--
-- Definition of function `f_speciesShort`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `f_speciesShort`(species CHAR(128)) RETURNS char(128) CHARSET utf8
BEGIN
   	DECLARE SpeciesShort CHAR(128);
	
    	SET species = REPLACE(species,'object/creature/player/shared_','object/creature/player/');
   		
		SELECT SUBSTRING_INDEX(TRIM(LEADING 'object/creature/player/' FROM species),'_',1) INTO SpeciesShort;
		RETURN speciesShort;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_CharacterNameCreate`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
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

  SELECT f_speciesShort(base_model_string) INTO shortSpecies;

  SELECT sf_GenerateFirstName(shortSpecies, gender) INTO gen_firstname;
  SELECT sf_GenerateLastName(shortSpecies, gender) INTO gen_lastname;

  IF shortSpecies = 'wookiee' THEN SET gen_lastname = NULL;
  END IF;

	SELECT CONCAT_WS(' ',gen_firstname,gen_lastname) INTO gen_fullname;

	RETURN gen_fullname;

	END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_CharacterNameDeveloperCheck`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_CharacterNameDeveloperCheck`(start_firstname CHAR(32)) RETURNS int(11)
BEGIN

	DECLARE check_name char(32);
	DECLARE check_value INT(11);
	DECLARE error_code  INT(11);

         SET error_code = 666;
	SELECT COUNT(*) from name_developer where LOWER(name) LIKE LOWER(start_firstname) INTO check_value;
	
	IF check_value > 0 THEN SET error_code = 0;
	END IF;
		
	RETURN error_code;

	END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_CharacterNameFictionalCheck`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_CharacterNameFictionalCheck`(start_firstname CHAR(32)) RETURNS int(11)
BEGIN

	DECLARE check_name char(32);
	DECLARE check_value INT(11);
	DECLARE error_code INT(11);

         SET error_code = 666;
	SELECT COUNT(*) from name_reserved where LOWER(name) LIKE LOWER(start_firstname) INTO check_value;
	
	IF check_value > 0 THEN SET error_code = 2;
	END IF;
		
	RETURN error_code;

	END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_CharacterNameInUseCheck`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_CharacterNameInUseCheck`(start_firstname CHAR(32)) RETURNS int(11)
BEGIN

	DECLARE check_name char(32);
	DECLARE check_value INT(11);
	DECLARE error_code INT(11);

         SET error_code = 666;
	SELECT COUNT(*) from characters where LOWER(firstname) LIKE LOWER(start_firstname) INTO check_value;
	
	IF check_value > 0 THEN SET error_code = 3;
	END IF;
		
	RETURN error_code;

	END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_CharacterNameProfaneCheck`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_CharacterNameProfaneCheck`(start_firstname CHAR(32)) RETURNS char(32) CHARSET utf8
BEGIN
	
	DECLARE check_name char(32);
	DECLARE check_value INT(11);
	DECLARE error_code char(32);
	
	SELECT COUNT(*) from name_profane where name LIKE start_firstname INTO check_value;
	
	IF check_value > 0 THEN SET error_code = 'name_declined_profane';
	END IF;
		
	RETURN error_code;

	END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_GenerateFirstName`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_GenerateFirstName`(start_species CHAR(16), start_gender INT(11)) RETURNS char(16) CHARSET utf8
BEGIN

  
  DECLARE gen_firstname CHAR(16);
  DECLARE raceId INT(8);

  
  SELECT id FROM species WHERE species_name = start_species INTO raceId;

  
  SELECT firstname FROM namegen_firstname WHERE species = raceId AND gender = start_gender ORDER BY RAND() LIMIT 1 INTO gen_firstname;

  RETURN gen_firstname;

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of function `sf_GenerateLastName`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` FUNCTION `sf_GenerateLastName`(start_species CHAR(16), start_gender INT(11)) RETURNS char(16) CHARSET utf8
BEGIN

  
  DECLARE gen_lastname CHAR(16);
  DECLARE raceId INT(8);

  
  SELECT id FROM species WHERE species_name = start_species INTO raceId;

  
  SELECT lastname FROM namegen_lastname WHERE species = raceId AND gender = start_gender ORDER BY RAND() LIMIT 1 INTO gen_lastname;

  RETURN gen_lastname;

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_CharacterCreate`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CharacterCreate`(
	IN start_account_id INT,IN start_galaxy_id INT,IN start_firstname char(32),IN start_lastname char(32),
	IN start_profession char(64),IN start_city char(32),IN start_scale FLOAT,IN start_biography text(2048),
  IN start_appearance_customization TINYBLOB, IN start_hair_model CHAR(64), IN hair_customization TEXT(200), IN base_model_string CHAR(64))
charCreate:BEGIN

  --
  -- Declare Vars
  --

  DECLARE oX FLOAT;DECLARE oY FLOAT;DECLARE oZ FLOAT;DECLARE oW FLOAT;
  DECLARE race_id INT;
	DECLARE entity_id BIGINT(20);
  DECLARE player_id BIGINT(20);
  DECLARE character_id BIGINT(20);
  DECLARE character_parent_id BIGINT(20);
  DECLARE planet_name char(32);
	DECLARE profession_id INT;
	DECLARE start_planet INT;
	DECLARE start_x FLOAT;DECLARE start_y FLOAT;DECLARE start_z FLOAT;
	DECLARE shortSpecies CHAR(32);
    
  DECLARE shared_model_string CHAR(64);
  
  DECLARE model_position INT;
	DECLARE gender INT(3);
  DECLARE base_skill_id INT;
  DECLARE nameCheck INT;
	
  --
  -- Transactional Support
  --

DECLARE EXIT HANDLER FOR NOT FOUND
    BEGIN
      SET entity_id = 4;
      ROLLBACK;
      SELECT entity_id;
    END;

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
      SET entity_id = 4;
      ROLLBACK;
      SELECT entity_id;
    END;
-- 
--   DECLARE EXIT HANDLER FOR SQLWARNING
--     BEGIN
--     SET entity_id = 4;
--     ROLLBACK;
--     SELECT entity_id;
--   END;
  --
  -- Check the new character name for validity
  --

  SELECT sf_CharacterNameDeveloperCheck(start_firstname) INTO nameCheck;
    IF nameCheck <> 666 THEN
      SELECT(nameCheck);
      LEAVE charCreate;
    END IF;

  SELECT sf_CharacterNameFictionalCheck(start_firstname) INTO nameCheck;
    IF nameCheck <> 666 THEN
      SELECT(nameCheck);
      LEAVE charCreate;
    END IF;

  SELECT sf_CharacterNameInUseCheck(start_firstname) INTO nameCheck;
    IF nameCheck <> 666 THEN
      SELECT(nameCheck);
      LEAVE charCreate;
    END IF;

  --
  -- Set the gender
  --

  IF base_model_string like '%female%' THEN
    SET gender = 0;
  ELSE
    SET gender = 1;
  END IF;
  
  --
  -- Set defaults (battle fatigue, world orientation)
  --

  SET character_parent_id = 0;
  SET oX = 0;
  SET oY = 1;
  SET oZ = 0;
  SET oW = 0;
		
	--
	-- Transaction Start
	--

  START TRANSACTION;

  SELECT MAX(id) + 10 FROM entity INTO entity_id FOR UPDATE;

  IF entity_id IS NULL THEN
    SET entity_id = 8589934593;
  END IF;

-- 
     SELECT planet_id, x, y, z FROM starting_location WHERE location LIKE 'coronet' INTO start_planet, start_x, start_y, start_z;
-- 
     SELECT f_speciesShort(base_model_string) INTO shortSpecies;
-- 
--   SELECT health, strength, constitution, action, quickness, stamina, mind, focus, willpower FROM starting_attributes WHERE starting_attributes.species like shortSpecies AND starting_attributes.profession like start_profession INTO t_health, t_strength, t_constitution, t_action, t_quickness, t_stamina, t_mind, t_focus, t_willpower;
-- 
     SELECT id from species where species.species_name like shortSpecies into race_id;
-- 
-- 
   INSERT INTO entity VALUES (entity_id, NULL, character_parent_id);
   INSERT INTO characters(entity_id, createdAt,updatedAt,firstName,lastName,jediState,birthDate,archived) VALUES (entity_id, NOW(), NOW(), start_firstname, start_lastname, 0, NOW(),0);
   SELECT id from characters where characters.entity_id = entity_id INTO character_id;
   SELECT id from player where referenceId = start_account_id INTO player_id;
   INSERT INTO players_characters values (player_id, character_id);
   INSERT INTO transform(entity_id,x,y,z,oX,oY,oZ,oW,planet_id) VALUES (entity_id, start_x, start_y, start_z, oX, oY, oZ, oW, start_planet);
   INSERT INTO appearance(entity_id,baseModel,scale,gender,species,customization_data) VALUES (entity_id, base_model_string, start_scale, gender, shortSpecies, start_appearance_customization);
	--
	-- Commit Transaction
	--
	
  COMMIT;
   
	--
	-- Return new character ID
	--
	
  SELECT(entity_id);

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_CharacterDelete`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CharacterDelete`(IN character_id BIGINT)
BEGIN

	--
	-- Declare Vars
	--

	DECLARE check_value INT(11);

	DECLARE error_code INT(11);

	--
	-- Mark the character as archived & set the deletion date to be 90days out
	--

	SET error_code = 0;

	UPDATE characters SET archived = 1 WHERE id = character_id;

	UPDATE characters SET deletedAt = (NOW()) WHERE id = character_id;

	--
	-- Check to see if we maked the character for deletion and return proper exit code
	--

	SELECT COUNT(*) from characters WHERE id = character_id AND archived = 1 INTO check_value;

	IF check_value > 0 THEN SET error_code = 1;
	END IF;

	SELECT error_code;

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_GetLoginCharacter`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetLoginCharacter`(IN id BIGINT(20))
BEGIN
	SELECT A.entity_id, B.baseModel, B.gender, B.species, C.x, C.y, C.z, 
    C.oX, C.oY, C.oZ, C.oW, D.terrainMap
	FROM characters A
	INNER JOIN appearance B ON (A.entity_id = B.entity_id)
  INNER JOIN transform C on (A.entity_id = C.entity_id)
  INNER JOIN planet D on (C.planet_id = D.planet_id)
  WHERE A.entity_id = id and A.archived = 0;

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_GetPlayerFromSession`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetPlayerFromSession`(IN player_id BIGINT(20))
BEGIN

    SELECT session_key from player where id = player_id;

END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;

--
-- Definition of procedure `sp_ReturnAccountCharacters`
--

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_ReturnAccountCharacters`(IN account_id INT)
BEGIN
    SELECT A.id, A.firstName, A.lastName, A.jediState, D.baseModel   
    FROM characters A
    INNER JOIN player B ON (account_id = B.referenceId)
    INNER JOIN players_characters C ON (B.id = C.player_id)
    INNER JOIN appearance D ON (A.entity_id = D.entity_id)
    WHERE A.id = C.character_id and A.archived = 0;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
