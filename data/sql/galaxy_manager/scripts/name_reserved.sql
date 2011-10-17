# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:38:43
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy_manager.name_reserved
CREATE TABLE IF NOT EXISTS `name_reserved` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

# Dumping data for table galaxy_manager.name_reserved: ~9 rows (approximately)
DELETE FROM `name_reserved`;
/*!40000 ALTER TABLE `name_reserved` DISABLE KEYS */;
INSERT INTO `name_reserved` (`id`, `name`) VALUES
	(1, 'Luke'),
	(2, 'Darth'),
	(3, 'Vader'),
	(4, 'Leia'),
	(5, 'Chewbacca'),
	(6, 'Yoda'),
	(7, 'Emperor'),
	(8, 'stormtrooper'),
	(9, 'TK');
/*!40000 ALTER TABLE `name_reserved` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
