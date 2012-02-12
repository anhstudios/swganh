
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `skills_preclusion_skills` (
  `skill_id` int(11) NOT NULL,
  `preclusion_skill_id` int(11) NOT NULL,
  PRIMARY KEY (`skill_id`,`preclusion_skill_id`),
  KEY `IDX_F89ED6FB5585C142` (`skill_id`),
  KEY `IDX_F89ED6FBB334A428` (`preclusion_skill_id`),
  CONSTRAINT `FK_F89ED6FB5585C142` FOREIGN KEY (`skill_id`) REFERENCES `skill` (`id`),
  CONSTRAINT `FK_F89ED6FBB334A428` FOREIGN KEY (`preclusion_skill_id`) REFERENCES `skill` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `skills_preclusion_skills`;
    
/*!40000 ALTER TABLE `skills_preclusion_skills` DISABLE KEYS */;
INSERT INTO `skills_preclusion_skills` (`skill_id`,`preclusion_skill_id`) VALUES 
 (745,707),
 (707,745),
 (1031,1012),
 (1050,1012),
 (1012,1031),
 (1050,1031),
 (1012,1050),
 (1031,1050);
/*!40000 ALTER TABLE `skills_preclusion_skills` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
