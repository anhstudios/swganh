/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_AddCharacterToAccount`;
DELIMITER //
CREATE PROCEDURE `sp_AddCharacterToAccount`(IN `object_id` BIGINT, IN `account_id` INT)
BEGIN
	   DECLARE player_id BIGINT(20);
		-- PLAYER ACCOUNT
      SELECT id FROM player_account where account_id = reference_id INTO player_id;
      INSERT INTO player_accounts_creatures values (player_id, object_id);
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
