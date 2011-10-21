# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-21 00:47:35
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.types
CREATE TABLE IF NOT EXISTS `types` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `description` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8 COMMENT='These are all of the possible object types';

# Dumping data for table galaxy.types: 15 rows
DELETE FROM `types`;
/*!40000 ALTER TABLE `types` DISABLE KEYS */;
INSERT INTO `types` (`id`, `description`) VALUES
	(1, 'player'),
	(2, 'creature'),
	(3, 'tangible'),
	(4, 'intangible'),
	(5, 'cell'),
	(6, 'manufacture_schematic'),
	(7, 'mission'),
	(8, 'waypoint'),
	(9, 'building'),
	(10, 'factory_crate'),
	(11, 'installation'),
	(12, 'ship'),
	(13, 'weapon'),
	(14, 'guild'),
	(15, 'group');
/*!40000 ALTER TABLE `types` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
