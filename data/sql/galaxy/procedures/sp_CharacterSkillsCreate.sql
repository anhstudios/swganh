
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_CharacterSkillsCreate`;

DELIMITER //

CREATE PROCEDURE `sp_CharacterSkillsCreate`(
    IN character_id BIGINT,
    IN profession_id INT,
    IN race_id INT)
BEGIN

  declare loopEnd INT default 0;
  declare skill_id INT;
  declare cur_1 cursor for select skills_species_required.skill_id from skills_species_required where skills_species_required.species_id = race_id;
  declare continue handler for sqlstate '02000' set loopEnd = 1;

  INSERT INTO creatures_skills VALUES (character_id,profession_id);

  open cur_1;

  repeat

     fetch cur_1 into skill_id;

     if NOT loopEnd then
        if(skill_id is not NULL) then
           INSERT INTO creatures_skills VALUES (character_id,skill_id);
        end if;
     end if;

  until loopEnd end repeat;

  close cur_1;

END //
DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
