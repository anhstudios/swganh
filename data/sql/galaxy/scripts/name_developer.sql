
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `name_developer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
--
-- Dumping data for table `name_developer`
--

/*!40000 ALTER TABLE `name_developer` DISABLE KEYS */;
INSERT INTO `name_developer` (`id`,`name`) VALUES 
 (1,'snow'),
 (2,'htx'),
 (3,'wookiee'),
 (4,'jack'),
 (5,'rouse'),
 (6,'ketusringa'),
 (7,'yosh'),
 (8,'jay'),
 (9,'schmunzel'),
 (10,'ready'),
 (11,'spety'),
 (12,'narook'),
 (13,'tmr'),
 (14,'o_0'),
 (15,'starko'),
 (16,'cieto'),
 (18,'pope'),
 (19, 'apathy'),
 (20, 'deadlock'),
 (21, 'kronos');

/*!40000 ALTER TABLE `name_developer` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
