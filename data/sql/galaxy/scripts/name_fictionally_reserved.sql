
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.name_fictionally_reserved
DROP TABLE IF EXISTS `name_fictionally_reserved`;
CREATE TABLE IF NOT EXISTS `name_fictionally_reserved` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.name_fictionally_reserved: ~9 rows (approximately)
/*!40000 ALTER TABLE `name_fictionally_reserved` DISABLE KEYS */;
INSERT INTO `name_fictionally_reserved` (`id`, `name`) VALUES
	(1, 'luke'),
	(2, 'darth'),
	(3, 'vader'),
	(4, 'leia'),
	(5, 'chewbacca'),
	(6, 'yoda'),
	(7, 'emperor'),
	(8, 'stormtrooper'),
	(9, 'tk');
/*!40000 ALTER TABLE `name_fictionally_reserved` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
