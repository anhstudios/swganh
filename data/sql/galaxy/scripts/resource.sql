
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `resource` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `type` int(11) NOT NULL,
  `er` smallint(6) NOT NULL,
  `cr` smallint(6) NOT NULL,
  `cd` smallint(6) NOT NULL,
  `dr` smallint(6) NOT NULL,
  `fl` smallint(6) NOT NULL,
  `hr` smallint(6) NOT NULL,
  `ma` smallint(6) NOT NULL,
  `op` smallint(6) NOT NULL,
  `sr` smallint(6) NOT NULL,
  `ut` smallint(6) NOT NULL,
  `pe` smallint(6) NOT NULL,
  `spawn_date` datetime NOT NULL,
  `expiration_date` datetime NOT NULL,
  `active` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `resource`;
    
/*!40000 ALTER TABLE `resource` DISABLE KEYS */;
/*!40000 ALTER TABLE `resource` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
