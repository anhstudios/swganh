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

# Dumping structure for procedure galaxy_manager_entity.sp_CharacterCreate
DELIMITER //
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
     SELECT sf_SpeciesShort(base_model_string) INTO shortSpecies;
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

END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
