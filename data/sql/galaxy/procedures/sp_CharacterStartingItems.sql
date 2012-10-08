
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_CharacterStartingItems`;

DELIMITER //

CREATE PROCEDURE `sp_CharacterStartingItems`(
	IN owner_id BIGINT(20),
	IN race_id INT(8),
	IN profession_id INT(8),
	IN gender_id INT(3),
	IN scene_id INT)
BEGIN

    DECLARE object_id BIGINT(20);
	DECLARE tmpId BIGINT(20);
    DECLARE iff_template_id INT;
    DECLARE template CHAR(255);
    DECLARE loopEnd INT DEFAULT 0;
    DECLARE cur_1 CURSOR FOR SELECT creation_item.template FROM creation_item WHERE creation_item.species_id=race_id AND creation_item.gender=gender_id AND creation_item.profession_id=profession_id;
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET loopEnd = 1;

	OPEN cur_1;
        REPEAT
            FETCH cur_1 INTO template;
            IF NOT loopEnd THEN
                SELECT MAX(id) + 10 FROM object INTO object_id FOR UPDATE;

                SELECT swganh_static.iff_templates.id FROM swganh_static.iff_templates WHERE swganh_static.iff_templates.iff_template LIKE template INTO iff_template_id;

                INSERT INTO `object` VALUES (object_id, scene_id, owner_id, iff_template_id, 0,0,0,0,0,0,1, 0, '', '', '' ,0, NOW(), NOW(), null, 1413566031, -2, 1);
                INSERT INTO `tangible` VALUES (object_id, '', 0, 0, 100, 100, 0);
            END IF;
        UNTIL loopEnd END REPEAT;
    CLOSE cur_1;
END //
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
