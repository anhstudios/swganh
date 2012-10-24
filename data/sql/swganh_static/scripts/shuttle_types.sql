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

-- Dumping structure for table swganh_static.shuttle_types
DROP TABLE IF EXISTS `shuttle_types`;
CREATE TABLE IF NOT EXISTS `shuttle_types` (
  `id` int(11) unsigned NOT NULL DEFAULT '0',
  `object_string` char(255) NOT NULL DEFAULT 'object/creature/npc/theme_park/shared_player_shuttle.iff',
  `name` char(255) DEFAULT 'shuttle',
  `file` char(255) NOT NULL DEFAULT 'theme_park_name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.shuttle_types: ~4 rows (approximately)
/*!40000 ALTER TABLE `shuttle_types` DISABLE KEYS */;
INSERT INTO `shuttle_types` (`id`, `object_string`, `name`, `file`) VALUES
	(1, 'object/creature/npc/theme_park/shared_player_shuttle.iff', 'shuttle', 'theme_park_name'),
	(2, 'object/creature/npc/theme_park/shared_lambda_shuttle.iff', 'shuttle', 'theme_park_name'),
	(3, 'object/creature/npc/theme_park/shared_player_transport.iff', 'transport', 'theme_park_name'),
	(4, 'object/creature/npc/theme_park/shared_player_transport_theed_hangar.iff', 'transport', 'theme_park_name');
/*!40000 ALTER TABLE `shuttle_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
