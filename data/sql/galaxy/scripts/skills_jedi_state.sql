
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `skills_jedi_state` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `name` char(255) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

DELETE FROM `skills_jedi_state`;
    
/*!40000 ALTER TABLE `skills_jedi_state` DISABLE KEYS */;
INSERT INTO `skills_jedi_state` (`id`,`name`) VALUES 
 (0,'none'),
 (1,'forceSensitive'),
 (2,'jedi'),
 (4,'forceRankedLight'),
 (8,'forceRankedDark');
/*!40000 ALTER TABLE `skills_jedi_state` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
