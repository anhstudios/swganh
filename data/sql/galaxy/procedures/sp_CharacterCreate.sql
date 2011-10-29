# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-26 22:57:06
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_CharacterCreate
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CharacterCreate`(IN `start_account_id` INT, IN `start_galaxy_id` INT, IN `start_firstname` char(32), IN `start_lastname` char(32), IN `start_custom_name` char(50), IN `start_profession` char(64), IN `start_city` char(32), IN `start_scale` FLOAT, IN `start_biography` text(2048), IN `start_appearance_customization` TINYBLOB, IN `start_hair_model` CHAR(64), IN `hair_customization` TEXT(200), IN `base_model_string` CHAR(64))
charCreate:BEGIN

    DECLARE oX FLOAT;DECLARE oY FLOAT;DECLARE oZ FLOAT;DECLARE oW FLOAT;
    DECLARE race_id INT;
    DECLARE iff_template_id INT;
    DECLARE player_iff_template_id INT;
    DECLARE hair_iff_template_id INT;
    DECLARE object_id BIGINT(20);
    DECLARE player_id BIGINT(20);
    DECLARE character_id BIGINT(20);
    DECLARE parent_id BIGINT(20);
    DECLARE planet_name char(32);
    DECLARE profession_id INT;
    DECLARE start_scene INT;
    DECLARE start_x FLOAT;DECLARE start_y FLOAT;DECLARE start_z FLOAT;
    DECLARE shortSpecies CHAR(255);
    DECLARE longSpecies CHAR(255);
    DECLARE longHair CHAR(255);
    DECLARE shared_template_string CHAR(64);
    DECLARE model_position INT;
    DECLARE gender INT(3);
    DECLARE base_skill_id INT;
    DECLARE nameCheck INT;
    
    DECLARE health INT;
    DECLARE strength INT;
    DECLARE constitution INT;
    DECLARE action INT;
    DECLARE quickness INT;
    DECLARE stamina INT;
    DECLARE mind INT;
    DECLARE focus INT;
    DECLARE willpower INT;
    
    SELECT sf_CharacterNameInUseCheck(start_firstname) INTO nameCheck;
    IF nameCheck <> 666 THEN
        SELECT(nameCheck);
        LEAVE charCreate;
    END IF;
    
    IF base_model_string like '%female%' THEN
        SET gender = 1;
    ELSE
        SET gender = 0;
    END IF;

    SET parent_id = 0;
    SET oX = 0;
    SET oY = 1;
    SET oZ = 0;
    SET oW = 0;

    START TRANSACTION;
        SELECT MAX(id) + 10 FROM object INTO object_id FOR UPDATE;
        
        IF object_id IS NULL THEN
            SET object_id = 8589934593;
        END IF;
        
        
        SELECT scene_id, x, y, z FROM starting_location WHERE location LIKE 'coronet' INTO start_scene, start_x, start_y, start_z;
        
        SELECT id from skill where name like start_profession INTO profession_id;
        
	    SELECT sf_SpeciesShort(base_model_string) INTO shortSpecies;
	    SELECT id from species where species.name like shortSpecies into race_id;
        
	    SET longSpecies = REPLACE(base_model_string, 'object/creature/player/', 'object/creature/player/shared_');
	    SELECT iff_templates.id FROM iff_templates WHERE iff_templates.iff_template LIKE longSpecies INTO iff_template_id;
        
        SELECT iff_templates.id FROM iff_templates WHERE iff_templates.iff_template LIKE 'object/player/shared_player.iff' INTO player_iff_template_id;
        
	    SET longHair = REPLACE(start_hair_model, '/hair_', '/shared_hair_');
        
	    SELECT iff_templates.id FROM iff_templates WHERE iff_templates.iff_template LIKE longHair INTO hair_iff_template_id;
        
        SELECT creation_attributes.health, creation_attributes.strength, creation_attributes.constitution, creation_attributes.action, 
        creation_attributes.quickness, creation_attributes.stamina, creation_attributes.mind, creation_attributes.focus, creation_attributes.willpower 
        FROM creation_attributes WHERE creation_attributes.species_id = race_id AND creation_attributes.profession_id = profession_id
        INTO health, strength, constitution, action, quickness, stamina, mind, focus, willpower;
        
        INSERT INTO `object` VALUES (object_id, start_scene, parent_id, iff_template_id, start_x,start_y,start_z,oX,oY,oZ,oW, 0, 'player_species', concat('name_',shortSpecies), start_custom_name,0, NOW(), NOW(), null, 1129465167);
        INSERT INTO `tangible` VALUES (object_id, start_appearance_customization, 0, 0, 0, 0, 1);
        INSERT INTO `creature`(id, owner_id, bank_credits, cash_credits, posture, 
            scale, acceleration_base, acceleration_modifier, speed_base, speed_modifier, 
            run_speed, slope_modifier_angle, slope_modifier_percent, turn_radius, walking_speed,
            max_health, max_strength, max_constitution, max_action, max_quickness, max_stamina,
            max_mind, max_focus, max_willpower, current_health, current_strength, current_constitution,
            current_action, current_quickness, current_stamina, current_mind, current_focus,
            current_willpower)
	        VALUES (object_id, parent_id, 2000, 0, 0, start_scale, 1, 1, 1, 1, 5.75, 1, 1, 1, 1,
	            health, strength, constitution, action, quickness, stamina, mind, focus, willpower,
	            health, strength, constitution, action, quickness, stamina, mind, focus, willpower );
	    -- APPEARANCE
	    INSERT INTO `appearance` VALUES (object_id, scale, gender, shortSpecies, start_appearance_customization);
	    -- DATAPAD
	    --	INSERT INTO `object` VALUES (object_id + 2, start_scene, object_id, base_model_string, start_x,start_y,start_z,oX,oY,oZ,oW, 0, base_model_string, base_model_string, start_custom_name,0, NOW(), NOW(), null, start_custom_name + ' datapad');
	    -- INVENTORY
	    -- BANK
	    -- MISSION
	    -- HAIR
        INSERT INTO `object` VALUES (object_id + 6, start_scene, object_id, hair_iff_template_id, start_x,start_y,start_z,oX,oY,oZ,oW, 0, 'hair_detail', 'hair', '' ,0, NOW(), NOW(), null, 1413566031);
        INSERT INTO `tangible` VALUES (object_id + 6, hair_customization, 0, 0, 0, 0, 0);
  	    INSERT INTO `appearance` VALUES (object_id + 6, scale, gender, shortSpecies, hair_customization);
	    -- EQUIPED
	    -- PLAYER
	    INSERT INTO `object` VALUES (object_id + 1, start_scene, object_id, player_iff_template_id, start_x,start_y,start_z,oX,oY,oZ,oW, 0, 'string_id_table', '', start_custom_name,0, NOW(), NOW(), null, 1347174745);
	    INSERT INTO `player` (id, profession_tag, born_date, csr_tag, current_language, jedi_state)
	        VALUES (object_id + 1, start_profession, NOW(), 0, 0, 0);
	    -- PLAYER ACCOUNT
	    SELECT id FROM player_account where start_account_id = reference_id INTO player_id;
        INSERT INTO player_accounts_creatures values (player_id, object_id);
    COMMIT;
   
   CALL sp_CharacterStartingItems(object_id, race_id, profession_id, gender);

   SELECT(object_id);

END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
