
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `scene` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `label` varchar(255) DEFAULT NULL,
  `description` longtext NOT NULL,
  `terrain` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=45 DEFAULT CHARSET=latin1;

DELETE FROM `scene`;
    
/*!40000 ALTER TABLE `scene` DISABLE KEYS */;
INSERT INTO `scene` (`id`, `name`, `label`, `description`, `terrain`) VALUES
	(1, 'corellia', 'corellia', '', 'terrain/corellia.trn'),
	(2, 'dantooine', NULL, '', 'terrain/dantooine.trn'),
	(3, 'dathomir', NULL, '', 'terrain/dathomir.trn'),
	(4, 'endor', NULL, '', 'terrain/endor.trn'),
	(5, 'lok', NULL, '', 'terrain/lok.trn'),
	(6, 'naboo', NULL, '', 'terrain/naboo.trn'),
	(7, 'rori', NULL, '', 'terrain/rori.trn'),
	(8, 'talus', NULL, '', 'terrain/talus.trn'),
	(9, 'tatooine', NULL, '', 'terrain/tatooine.trn'),
	(10, 'yavin4', NULL, '', 'terrain/yavin4.trn'),
	(11, 'space_corellia', NULL, '', 'terrain/space_corellia.trn'),
	(12, 'space_corellia_2', NULL, '', 'terrain/space_corellia_2.trn'),
	(13, 'space_dantooine', NULL, '', 'terrain/space_dantooine.trn'),
	(14, 'space_dathomir', NULL, '', 'terrain/space_dathomir.trn'),
	(15, 'space_endor', NULL, '', 'terrain/space_endor.trn'),
	(16, 'space_env', NULL, '', 'terrain/space_env.trn'),
	(17, 'space_halos', NULL, '', 'terrain/space_halos.trn'),
	(18, 'space_heavy1', NULL, '', 'terrain/space_heavy1.trn'),
	(19, 'space_light1', NULL, '', 'terrain/space_light1.trn'),
	(20, 'space_lok', NULL, '', 'terrain/space_lok.trn'),
	(21, 'space_naboo', NULL, '', 'terrain/space_naboo.trn'),
	(22, 'space_naboo_2', NULL, '', 'terrain/space_naboo_2.trn'),
	(23, 'space_tatooine', NULL, '', 'terrain/space_tatooine.trn'),
	(24, 'space_tatooine_2', NULL, '', 'terrain/space_tatooine_2.trn'),
	(25, 'space_yavin4', NULL, '', 'terrain/space_yavin4.trn'),
	(26, '09', NULL, '', 'terrain/09.trn'),
	(27, '10', NULL, '', 'terrain/10.trn'),
	(28, '11', NULL, '', 'terrain/11.trn'),
	(29, 'character_farm', NULL, '', 'terrain/character_farm.trn'),
	(30, 'cinco_city_test_m5', NULL, '', 'terrain/cinco_city_test_m5.trn'),
	(31, 'creature_test', NULL, '', 'terrain/creature_test.trn'),
	(32, 'dungeon1', NULL, '', 'terrain/dungeon1.trn'),
	(33, 'endor_asommers', NULL, '', 'terrain/endor_asommers.trn'),
	(34, 'floratest', NULL, '', 'terrain/floratest.trn'),
	(35, 'godclient_test', NULL, '', 'terrain/godclient_test.trn'),
	(36, 'otoh_gunga', NULL, '', 'terrain/otoh_gunga.trn'),
	(37, 'rivertest', NULL, '', 'terrain/rivertest.trn'),
	(38, 'runtimerules', NULL, '', 'terrain/runtimerules.trn'),
	(39, 'simple', NULL, '', 'terrain/simple.trn'),
	(40, 'space_09', NULL, '', 'terrain/space_09.trn'),
	(41, 'test_wearables', NULL, '', 'terrain/test_wearables.trn'),
	(42, 'tutorial', NULL, '', 'terrain/tutorial.trn'),
	(43, 'taanab', NULL, '', 'terrain/taanab.trn'),
	(44, 'dagobah', NULL, '', 'terrain/dagobah.trn');
/*!40000 ALTER TABLE `scene` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
