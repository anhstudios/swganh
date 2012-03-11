# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2012-03-10 18:37:12
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_CreateSWGANHBots
DROP PROCEDURE IF EXISTS `sp_CreateSWGANHBots`;
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CreateSWGANHBots`(IN bots int, IN firstId int, IN start_city char(32))
BEGIN
	DECLARE new_account_id BIGINT(20);
	DECLARE new_username CHAR(16);
	DECLARE lastName CHAR(16);
	DECLARE new_userpass CHAR(128);
	DECLARE new_character_id BIGINT(22);
	DECLARE mod_username CHAR(32);
	DECLARE loop_counter int;
	DECLARE bot_id BIGINT(20);
	DECLARE loopEnd INT default 0;
	DECLARE first_bot_name CHAR(32);

	DECLARE gender_id INT(11);
	DECLARE profession_id INT(11);
	DECLARE species_id INT(11);
	DECLARE species_out CHAR(24);
	 
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET loopEnd = 1;

	SET new_username = 'bot#';
	SET bot_id = firstId;

	SET loop_counter = 0;

	IF start_city = '' THEN
		SET start_city = 'bestine';
	END IF;

START TRANSACTION;
	loop1: LOOP

	-- This will ensure a number of 0 bots will never be executed.
	IF loop_counter = bots THEN
		LEAVE loop1;
	END IF;

	SET mod_username = CONCAT(new_username, bot_id);
	SET new_userpass = CONCAT(mod_username, 'swganh');

	Call galaxy_manager.sp_CreateBotAccount(mod_username, new_userpass);

	SELECT LAST_INSERT_ID() INTO new_account_id;
	
	CALL galaxy.sp_CreatePlayerAccount(new_account_id);
	
	-- To obtain a random integer R in the range i <= R < j, use the expression FLOOR(i + RAND() * (j  i)). 
	SELECT FLOOR(RAND() * (2)) INTO gender_id;
	SELECT FLOOR(RAND() * (6)) INTO profession_id;
	SELECT FLOOR(RAND() * (10)) INTO species_id;

	if species_id = 8 then
		SET species_id = 33;
	elseif species_id = 9 then
		SET species_id = 49;
  elseif species_id = 4 then
		SET species_id = 1;
	end if;
	
	SELECT name FROM species WHERE id=species_id INTO species_out;
	CALL sp_GenerateLastName(species_out, gender_id, @lastName);
	
	if species_id = 1 then
		-- human
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/human_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/human_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/human_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/human_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/human_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/human_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/human_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/human_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/human_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/human_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/human_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/human_male.iff', @obj_id);
			end if;
		end if;	
		
	elseif species_id = 2 then
		-- rodian
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/rodian_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/rodian_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/rodian_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/rodian_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/rodian_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/rodian_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/rodian_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/rodian_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/rodian_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/rodian_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/rodian_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/rodian_male.iff', @obj_id);
			end if;
		end if;	

	elseif species_id = 3 then
		-- trandoshan
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/trandoshan_male.iff', @obj_id);
			end if;
		end if;	

	elseif species_id = 4 then
		-- moncal
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/moncal_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/moncal_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/moncal_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/moncal_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/moncal_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/moncal_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/moncal_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/moncal_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/moncal_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/moncal_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/moncal_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/moncal_male.iff', @obj_id);
			end if;
		end if;	

	elseif species_id = 5 then
		-- wookiee
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/wookiee_male.iff', @obj_id);
			end if;
		end if;	
		
	elseif species_id = 6 then
		-- bothan
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/bothan_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/bothan_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/bothan_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/bothan_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/bothan_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/bothan_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/bothan_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/bothan_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/bothan_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/bothan_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/bothan_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/bothan_male.iff', @obj_id);
			end if;
		end if;	
		
	elseif species_id = 7 then
		-- twilek
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/twilek_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/twilek_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/twilek_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/twilek_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/twilek_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/twilek_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/twilek_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/twilek_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/twilek_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/twilek_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/twilek_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/twilek_male.iff', @obj_id);
			end if;
		end if;	
		
	elseif species_id = 8 then
		-- zabrak
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/zabrak_male.iff', @obj_id);
			end if;
		end if;	
		
	elseif species_id = 9 then
		-- ithorian
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 0.932384, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.914971, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_female.iff', @obj_id);
			elseif profession_id = 2 then				                         
				-- entertainer                                                   
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.923745, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_female.iff', @obj_id);
			elseif profession_id = 3 then				                          
				-- marksman                                                       
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.976468, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_female.iff', @obj_id);
			elseif profession_id = 4 then				                           
				-- medic                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.988579, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.012185, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','crafting_artisan', start_city, 1.042303, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_male.iff', @obj_id);
			elseif profession_id = 1 then				                        
				-- brawler                                                      
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.931314, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_male.iff', @obj_id);
			elseif profession_id = 2 then				                          
				-- entertainer                                                    
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 1.026153, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_male.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 1.004691, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_male.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.934483, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_male.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 1.059221, NULL, 0, NULL, NULL, 'object/creature/player/ithorian_male.iff', @obj_id);
			end if;
		end if;	
		
	elseif species_id = 10 then
		-- sullustan
		if gender_id = 0 then
			-- female
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '', 'crafting_artisan', start_city, 0.946259, NULL, 0, NULL, NULL, 'object/creature/player/sullustan_female.iff', @obj_id);
			elseif profession_id = 1 then				                          
				-- brawler                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_brawler', start_city, 0.989145, NULL, 0, NULL, NULL, 'object/creature/player/sullustan_female.iff', @obj_id);
			elseif profession_id = 2 then				                        
				-- entertainer                                                  
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','social_entertainer', start_city, 0.928251,NULL, 0, NULL, NULL,'object/creature/player/sullustan_female.iff', @obj_id);
			elseif profession_id = 3 then				                           
				-- marksman                                                        
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','combat_marksman', start_city, 0.990826,NULL, 0, NULL, NULL,'object/creature/player/sullustan_female.iff', @obj_id);
			elseif profession_id = 4 then				                          
				-- medic                                                          
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','science_medic', start_city, 0.954236,NULL, 0, NULL, NULL,'object/creature/player/sullustan_female.iff', @obj_id);
			elseif profession_id = 5 then				                           
				-- scout                                                           
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, '','outdoors_scout', start_city, 0.926948,NULL, 0, NULL, NULL,'object/creature/player/sullustan_female.iff', @obj_id);
			end if;
		else
			-- male
			if profession_id = 0 then
				-- artisan
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, 'crafting_artisan', start_city, 1.052737,NULL, 0, NULL, NULL,'object/creature/player/sullustan_male.iff', @obj_id);
			elseif profession_id = 1 then				
				-- brawler
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, 'combat_brawler', start_city, 1.014336,NULL, 0, NULL, NULL,'object/creature/player/sullustan_male.iff', @obj_id);
			elseif profession_id = 2 then				
				-- entertainer
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, 'social_entertainer', start_city, 1.007075,NULL, 0, NULL, NULL,'object/creature/player/sullustan_male.iff', @obj_id);
			elseif profession_id = 3 then				
				-- marksman
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, 'combat_marksman', start_city, 1.055619,NULL, 0, NULL, NULL,'object/creature/player/sullustan_male.iff', @obj_id);
			elseif profession_id = 4 then				
				-- medic
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, 'science_medic', start_city, 0.948675,NULL, 0, NULL, NULL,'object/creature/player/sullustan_male.iff', @obj_id);
			elseif profession_id = 5 then				
				-- scout
				CALL sp_CharacterCreate(new_account_id, 1, mod_username, lastName, 'outdoors_scout', start_city, 1.019355,NULL, 0, NULL, NULL,'object/creature/player/sullustan_male.iff', @obj_id);
			end if;
		end if;
		
	end if;	
		
	SET bot_id = bot_id + 1;
	SET loop_counter = loop_counter + 1;

	END LOOP loop1;

COMMIT;

END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
