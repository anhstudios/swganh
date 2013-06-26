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

-- Dumping structure for table galaxy.scene
DROP TABLE IF EXISTS `scene`;
CREATE TABLE IF NOT EXISTS `scene` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `label` varchar(255) DEFAULT NULL,
  `description` longtext NOT NULL,
  `terrain` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=45 DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.scene: ~44 rows (approximately)
/*!40000 ALTER TABLE `scene` DISABLE KEYS */;
INSERT INTO `scene` (`id`, `name`, `label`, `description`, `terrain`) VALUES
	(NULL, 'bad', 'bad', 'Null scene for object prototypes', 'bad'),
	(1, 'corellia', 'corellia', '', 'terrain/corellia.trn'),
	(2, 'dantooine', 'dantooine', '', 'terrain/dantooine.trn'),
	(3, 'dathomir', 'dathomir', '', 'terrain/dathomir.trn'),
	(4, 'endor', 'endor', '', 'terrain/endor.trn'),
	(5, 'lok', 'lok', '', 'terrain/lok.trn'),
	(6, 'naboo', 'naboo', '', 'terrain/naboo.trn'),
	(7, 'rori', 'rori', '', 'terrain/rori.trn'),
	(8, 'talus', 'talus', '', 'terrain/talus.trn'),
	(9, 'tatooine', 'tatooine', '', 'terrain/tatooine.trn'),
	(10, 'yavin4', 'yavin4', '', 'terrain/yavin4.trn'),
	(11, 'space_corellia', 'space_corellia', '', 'terrain/space_corellia.trn'),
	(12, 'space_corellia_2', 'space_corellia_2', '', 'terrain/space_corellia_2.trn'),
	(13, 'space_dantooine', 'space_dantooine', '', 'terrain/space_dantooine.trn'),
	(14, 'space_dathomir', 'space_dathomir', '', 'terrain/space_dathomir.trn'),
	(15, 'space_endor', 'space_endor', '', 'terrain/space_endor.trn'),
	(16, 'space_env', 'space_env', '', 'terrain/space_env.trn'),
	(17, 'space_halos', 'space_halos', '', 'terrain/space_halos.trn'),
	(18, 'space_heavy1', 'space_heavy1', '', 'terrain/space_heavy1.trn'),
	(19, 'space_light1', 'space_light1', '', 'terrain/space_light1.trn'),
	(20, 'space_lok', 'space_lok', '', 'terrain/space_lok.trn'),
	(21, 'space_naboo', 'space_naboo', '', 'terrain/space_naboo.trn'),
	(22, 'space_naboo_2', 'space_naboo_2', '', 'terrain/space_naboo_2.trn'),
	(23, 'space_tatooine', 'space_tatooine', '', 'terrain/space_tatooine.trn'),
	(24, 'space_tatooine_2', 'space_tatooine_2', '', 'terrain/space_tatooine_2.trn'),
	(25, 'space_yavin4', 'space_yavin4', '', 'terrain/space_yavin4.trn'),
	(26, '09', '09', '', 'terrain/09.trn'),
	(27, '10', '10', '', 'terrain/10.trn'),
	(28, '11', '11', '', 'terrain/11.trn'),
	(29, 'character_farm', 'character_farm', '', 'terrain/character_farm.trn'),
	(30, 'cinco_city_test_m5', 'cinco_city_test_m5', '', 'terrain/cinco_city_test_m5.trn'),
	(31, 'creature_test', 'creature_test', '', 'terrain/creature_test.trn'),
	(32, 'dungeon1', 'dungeon1', '', 'terrain/dungeon1.trn'),
	(33, 'endor_asommers', 'endor_asommers', '', 'terrain/endor_asommers.trn'),
	(34, 'floratest', 'floratest', '', 'terrain/floratest.trn'),
	(35, 'godclient_test', 'godclient_test', '', 'terrain/godclient_test.trn'),
	(36, 'otoh_gunga', 'otoh_gunga', '', 'terrain/otoh_gunga.trn'),
	(37, 'rivertest', 'rivertest', '', 'terrain/rivertest.trn'),
	(38, 'runtimerules', 'runtimerules', '', 'terrain/runtimerules.trn'),
	(39, 'simple', 'simple', '', 'terrain/simple.trn'),
	(40, 'space_09', 'space_09', '', 'terrain/space_09.trn'),
	(41, 'test_wearables', 'test_wearables', '', 'terrain/test_wearables.trn'),
	(42, 'tutorial', 'tutorial', '', 'terrain/tutorial.trn'),
	(43, 'taanab', 'taanab', '', 'terrain/taanab.trn'),
	(44, 'dagobah', 'dagobah', '', 'terrain/dagobah.trn');
/*!40000 ALTER TABLE `scene` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
