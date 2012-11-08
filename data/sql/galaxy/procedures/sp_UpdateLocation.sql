
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for procedure galaxy.sp_UpdateLocation
DROP PROCEDURE IF EXISTS `sp_UpdateLocation`;
DELIMITER //
CREATE PROCEDURE `sp_UpdateLocation`(IN `location_id_` INT(10), IN `name_` TEXT, IN `planet_id_` INT(11), 
									 IN `category_` INT(10), IN `sub_category_` INT(10), IN `x_` DOUBLE, IN `y_` DOUBLE)
BEGIN
DECLARE category_id_ INT(10);
IF NOT EXISTS(SELECT id FROM planetmap WHERE id = location_id_ AND planet_id = planet_id_ AND `name` = name_) THEN
	-- Search for the location category, set to 1 (nothing) if it doesn't exist.
	SET category_id_ = (SELECT id FROM swganh_static.planetmapcategory WHERE main = category_ AND sub = sub_category_);
	
	IF (category_id_ IS NULL) THEN
		SET category_id_ = 1;
	END IF;

	SELECT category_id_;

	INSERT INTO planetmap SET id = location_id_, `name` = name_, planet_id = planet_id_, category_id = category_id_, x = x_, y = y_, z = 0, icon = 0;
END IF;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
