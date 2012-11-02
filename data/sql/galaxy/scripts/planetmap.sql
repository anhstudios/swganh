-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:18
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.planetmap
DROP TABLE IF EXISTS `planetmap`;
CREATE TABLE IF NOT EXISTS `planetmap` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `category_id` int(10) unsigned NOT NULL,
  `planet_id` int(11) NOT NULL,
  `x` float(10,2) NOT NULL,
  `y` float(10,2) NOT NULL,
  `z` float(10,2) NOT NULL,
  `icon` int(11) NOT NULL,
  `name` text NOT NULL,
  PRIMARY KEY (`id`,`category_id`),
  KEY `planetmapcategory` (`id`),
  KEY `category_id` (`category_id`),
  KEY `planet_id` (`planet_id`),
  CONSTRAINT `planetmap_ibfk_1` FOREIGN KEY (`category_id`) REFERENCES swganh_static.planetmapcategory (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=999999681 DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.planetmap: ~0 rows (approximately)
/*!40000 ALTER TABLE `planetmap` DISABLE KEYS */;
/*!40000 ALTER TABLE `planetmap` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
