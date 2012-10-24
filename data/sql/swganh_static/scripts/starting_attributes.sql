-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:19
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.starting_attributes
DROP TABLE IF EXISTS `starting_attributes`;
CREATE TABLE IF NOT EXISTS `starting_attributes` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `species` varchar(64) NOT NULL,
  `profession` varchar(64) NOT NULL,
  `health` int(11) unsigned NOT NULL,
  `strength` int(11) unsigned NOT NULL,
  `constitution` int(11) unsigned NOT NULL,
  `action` int(11) unsigned NOT NULL,
  `quickness` int(11) unsigned NOT NULL,
  `stamina` int(11) unsigned NOT NULL,
  `mind` int(11) unsigned NOT NULL,
  `focus` int(11) unsigned NOT NULL,
  `willpower` int(11) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=61 DEFAULT CHARSET=utf8 COMMENT='Starting HAM attributes';

-- Dumping data for table swganh_static.starting_attributes: ~60 rows (approximately)
/*!40000 ALTER TABLE `starting_attributes` DISABLE KEYS */;
INSERT INTO `starting_attributes` (`id`, `species`, `profession`, `health`, `strength`, `constitution`, `action`, `quickness`, `stamina`, `mind`, `focus`, `willpower`) VALUES
	(1, 'human', 'science_medic', 700, 400, 400, 800, 400, 400, 1100, 600, 600),
	(2, 'bothan', 'science_medic', 600, 300, 300, 1000, 600, 400, 1100, 600, 500),
	(3, 'moncal', 'science_medic', 600, 300, 300, 700, 300, 450, 1300, 800, 650),
	(4, 'rodian', 'science_medic', 600, 300, 300, 900, 500, 750, 1000, 500, 550),
	(5, 'trandoshan', 'science_medic', 850, 600, 700, 700, 300, 300, 1000, 500, 600),
	(6, 'twilek', 'science_medic', 800, 300, 600, 1050, 700, 400, 800, 350, 400),
	(7, 'wookiee', 'science_medic', 950, 650, 450, 900, 400, 400, 1100, 650, 600),
	(8, 'zabrak', 'science_medic', 800, 300, 300, 1000, 300, 300, 1000, 500, 900),
	(9, 'ithorian', 'science_medic', 600, 300, 300, 700, 300, 450, 1300, 800, 550),
	(10, 'sullustan', 'science_medic', 800, 300, 300, 1000, 300, 300, 1000, 500, 800),
	(11, 'human', 'outdoors_scout', 900, 400, 450, 900, 500, 500, 800, 450, 500),
	(12, 'bothan', 'outdoors_scout', 800, 300, 350, 1100, 700, 500, 800, 450, 400),
	(13, 'moncal', 'outdoors_scout', 800, 300, 350, 800, 400, 550, 1000, 650, 550),
	(14, 'rodian', 'outdoors_scout', 800, 300, 350, 1000, 600, 850, 700, 350, 450),
	(15, 'trandoshan', 'outdoors_scout', 1050, 600, 750, 800, 400, 400, 700, 350, 500),
	(16, 'twilek', 'outdoors_scout', 800, 300, 600, 1050, 700, 400, 800, 350, 400),
	(17, 'wookiee', 'outdoors_scout', 1150, 650, 500, 1000, 500, 500, 800, 500, 500),
	(18, 'zabrak', 'outdoors_scout', 1000, 300, 350, 1100, 400, 400, 700, 350, 800),
	(19, 'ithorian', 'outdoors_scout', 800, 300, 350, 800, 400, 550, 1000, 650, 550),
	(20, 'sullustan', 'outdoors_scout', 1000, 300, 350, 1100, 400, 400, 700, 350, 800),
	(21, 'human', 'crafting_artisan', 700, 400, 400, 900, 500, 400, 1000, 500, 600),
	(22, 'bothan', 'crafting_artisan', 600, 300, 300, 1100, 700, 400, 1000, 500, 500),
	(23, 'moncal', 'crafting_artisan', 600, 300, 300, 800, 400, 450, 1200, 700, 650),
	(24, 'rodian', 'crafting_artisan', 600, 300, 300, 1000, 600, 750, 900, 400, 550),
	(25, 'trandoshan', 'crafting_artisan', 850, 600, 700, 800, 400, 300, 900, 400, 600),
	(26, 'twilek', 'crafting_artisan', 600, 300, 550, 1050, 700, 300, 1000, 400, 500),
	(27, 'wookiee', 'crafting_artisan', 950, 650, 450, 1000, 500, 500, 1000, 550, 600),
	(28, 'zabrak', 'crafting_artisan', 800, 300, 300, 1100, 400, 300, 900, 400, 900),
	(29, 'ithorian', 'crafting_artisan', 600, 300, 300, 800, 400, 450, 1200, 700, 650),
	(30, 'sullustan', 'crafting_artisan', 800, 300, 300, 1100, 400, 300, 900, 400, 900),
	(31, 'human', 'combat_brawler', 1100, 600, 500, 900, 450, 450, 600, 400, 400),
	(32, 'bothan', 'combat_brawler', 1000, 500, 400, 1100, 650, 450, 600, 400, 300),
	(33, 'moncal', 'combat_brawler', 1000, 500, 400, 800, 350, 500, 800, 600, 450),
	(34, 'rodian', 'combat_brawler', 1000, 500, 400, 1000, 550, 800, 500, 300, 350),
	(35, 'trandoshan', 'combat_brawler', 1250, 800, 800, 800, 350, 350, 500, 300, 400),
	(36, 'twilek', 'combat_brawler', 1000, 500, 650, 1050, 650, 350, 600, 300, 300),
	(37, 'wookiee', 'combat_brawler', 1350, 850, 550, 1000, 450, 450, 600, 450, 400),
	(38, 'zabrak', 'combat_brawler', 1200, 500, 400, 1100, 350, 350, 500, 300, 700),
	(39, 'ithorian', 'combat_brawler', 1000, 500, 400, 800, 350, 500, 800, 600, 450),
	(40, 'sullustan', 'combat_brawler', 1200, 500, 400, 1100, 350, 350, 500, 300, 700),
	(41, 'human', 'social_entertainer', 600, 400, 400, 1100, 500, 500, 900, 500, 500),
	(42, 'bothan', 'social_entertainer', 500, 300, 300, 1300, 700, 500, 900, 500, 400),
	(43, 'moncal', 'social_entertainer', 500, 300, 300, 1000, 400, 550, 1100, 700, 550),
	(44, 'rodian', 'social_entertainer', 500, 300, 300, 1200, 600, 850, 800, 400, 450),
	(45, 'trandoshan', 'social_entertainer', 750, 600, 700, 1000, 400, 400, 800, 400, 500),
	(46, 'twilek', 'social_entertainer', 500, 300, 550, 1250, 700, 400, 900, 400, 400),
	(47, 'wookiee', 'social_entertainer', 850, 650, 450, 1200, 500, 500, 900, 550, 500),
	(48, 'zabrak', 'social_entertainer', 700, 300, 300, 1300, 400, 400, 800, 400, 800),
	(49, 'ithorian', 'social_entertainer', 500, 300, 300, 1000, 400, 550, 1100, 700, 550),
	(50, 'sullustan', 'social_entertainer', 700, 300, 300, 1300, 400, 400, 800, 400, 800),
	(51, 'human', 'combat_marksman', 1100, 550, 400, 900, 550, 400, 700, 400, 400),
	(52, 'bothan', 'combat_marksman', 1000, 450, 300, 1100, 750, 400, 700, 400, 400),
	(53, 'moncal', 'combat_marksman', 1000, 450, 300, 800, 450, 450, 900, 600, 450),
	(54, 'rodian', 'combat_marksman', 1000, 450, 300, 1000, 650, 750, 600, 300, 350),
	(55, 'trandoshan', 'combat_marksman', 1250, 750, 700, 800, 450, 300, 600, 300, 400),
	(56, 'twilek', 'combat_marksman', 1000, 450, 550, 1050, 750, 300, 700, 300, 300),
	(57, 'wookiee', 'combat_marksman', 1350, 800, 450, 1000, 550, 400, 700, 450, 400),
	(58, 'zabrak', 'combat_marksman', 1200, 450, 300, 1100, 450, 300, 600, 300, 700),
	(59, 'ithorian', 'combat_marksman', 1000, 450, 300, 800, 450, 450, 900, 600, 450),
	(60, 'sullustan', 'combat_marksman', 1200, 450, 300, 1100, 450, 300, 600, 300, 700);
/*!40000 ALTER TABLE `starting_attributes` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
