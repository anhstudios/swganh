-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.27-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-02 14:36:44
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for procedure galaxy.sp_GetCharacterIdByName
DROP PROCEDURE IF EXISTS `sp_GetCharacterIdByName`;
DELIMITER //
CREATE PROCEDURE `sp_GetCharacterIdByName`(IN `in_name` VARCHAR(50), IN `in_type` INT)
BEGIN
SELECT id FROM object where custom_name like in_name and type_id = in_type;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
