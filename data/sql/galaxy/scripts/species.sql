-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:00
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.species
DROP TABLE IF EXISTS `species`;
CREATE TABLE IF NOT EXISTS `species` (
  `id` int(11) NOT NULL,
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

-- Dumping data for table galaxy.species: ~10 rows (approximately)
/*!40000 ALTER TABLE `species` DISABLE KEYS */;
INSERT INTO `species` (`id`, `name`, `health_min`, `health_max`, `strength_min`, `strength_max`, `constitution_min`, `constitution_max`, `action_min`, `action_max`, `quickness_min`, `quickness_max`, `stamina_min`, `stamina_max`, `mind_min`, `mind_max`, `focus_min`, `focus_max`, `willpower_min`, `willpower_max`, `total`) VALUES
	(0, 'human', 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 5400),
	(1, 'rodian', 300, 1000, 300, 500, 300, 400, 300, 1200, 300, 650, 450, 850, 300, 1000, 300, 500, 350, 550, 5400),
	(2, 'trandoshan', 550, 1250, 600, 800, 700, 800, 300, 1000, 300, 450, 300, 400, 300, 1000, 300, 500, 300, 600, 5550),
	(3, 'moncal', 300, 1000, 300, 500, 300, 400, 300, 1000, 300, 450, 450, 550, 600, 1300, 600, 800, 450, 650, 5400),
	(4, 'wookiee', 650, 1350, 650, 850, 450, 550, 500, 1200, 400, 550, 400, 500, 400, 1100, 450, 650, 400, 600, 6100),
	(5, 'bothan', 300, 1000, 300, 500, 300, 400, 600, 1300, 600, 750, 400, 500, 400, 1100, 400, 600, 300, 500, 5400),
	(6, 'twilek', 300, 1000, 300, 500, 550, 650, 550, 1250, 600, 750, 300, 400, 400, 1100, 300, 500, 300, 500, 5400),
	(7, 'zabrak', 500, 1200, 300, 500, 300, 400, 600, 1300, 300, 450, 300, 400, 300, 1000, 300, 500, 700, 900, 5400),
	(33, 'ithorian', 300, 1400, 300, 600, 300, 500, 600, 1100, 300, 750, 300, 500, 400, 1300, 400, 600, 300, 500, 5400),
	(49, 'sullustan', 300, 1200, 300, 500, 300, 400, 600, 1400, 300, 750, 300, 500, 400, 1200, 400, 600, 300, 600, 5400);
/*!40000 ALTER TABLE `species` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
