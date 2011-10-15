# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-14 14:04:29
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.species
DROP TABLE IF EXISTS `species`;
CREATE TABLE IF NOT EXISTS `species` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `health_min` int(11) NOT NULL,
  `health_max` int(11) NOT NULL,
  `strength_min` int(11) NOT NULL,
  `strength_max` int(11) NOT NULL,
  `constitution_min` int(11) NOT NULL,
  `constitution_max` int(11) NOT NULL,
  `action_min` int(11) NOT NULL,
  `action_max` int(11) NOT NULL,
  `quickness_min` int(11) NOT NULL,
  `quickness_max` int(11) NOT NULL,
  `stamina_min` int(11) NOT NULL,
  `stamina_max` int(11) NOT NULL,
  `mind_min` int(11) NOT NULL,
  `mind_max` int(11) NOT NULL,
  `focus_min` int(11) NOT NULL,
  `focus_max` int(11) NOT NULL,
  `willpower_min` int(11) NOT NULL,
  `willpower_max` int(11) NOT NULL,
  `total` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.species: ~0 rows (approximately)
DELETE FROM `species`;
/*!40000 ALTER TABLE `species` DISABLE KEYS */;
/*!40000 ALTER TABLE `species` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
