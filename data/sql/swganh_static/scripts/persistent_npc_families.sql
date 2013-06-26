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

-- Dumping structure for table swganh_static.persistent_npc_families
DROP TABLE IF EXISTS `persistent_npc_families`;
CREATE TABLE IF NOT EXISTS `persistent_npc_families` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 ROW_FORMAT=REDUNDANT;

-- Dumping data for table swganh_static.persistent_npc_families: ~11 rows (approximately)
/*!40000 ALTER TABLE `persistent_npc_families` DISABLE KEYS */;
INSERT INTO `persistent_npc_families` (`id`, `name`) VALUES
	(1, 'trainers'),
	(2, 'fillers'),
	(3, 'quest_givers'),
	(4, 'junk_dealers'),
	(5, 'chassis_dealers'),
	(6, 'recruiters'),
	(7, 'attackable_objects'),
	(8, 'attackable_creatures'),
	(9, 'natural_lairs'),
	(10, 'mission_lairs'),
	(11, 'invisible_lairs');
/*!40000 ALTER TABLE `persistent_npc_families` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
