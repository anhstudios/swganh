-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:17
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.faction
DROP TABLE IF EXISTS `faction`;
CREATE TABLE IF NOT EXISTS `faction` (
  `id` int(8) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Faction ID',
  `name` varchar(32) NOT NULL COMMENT 'Faction Name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=79 DEFAULT CHARSET=utf8 COMMENT='Faction types';

-- Dumping data for table swganh_static.faction: ~78 rows (approximately)
/*!40000 ALTER TABLE `faction` DISABLE KEYS */;
INSERT INTO `faction` (`id`, `name`) VALUES
	(1, 'neutral'),
	(2, 'rebel'),
	(3, 'imperial'),
	(4, 'aakuans'),
	(5, 'afarathu'),
	(6, 'alkhara'),
	(7, 'bandit'),
	(8, 'beldonnas_league'),
	(9, 'binayre'),
	(10, 'bloodrazor'),
	(11, 'borvo'),
	(12, 'canyon_corsair'),
	(13, 'cobral'),
	(14, 'cor_swoop'),
	(15, 'corsec'),
	(16, 'dantari_raiders'),
	(17, 'desert_demon'),
	(18, 'donkuwah_tribe'),
	(19, 'drall'),
	(20, 'endor_marauder'),
	(21, 'fed_dub'),
	(22, 'flail'),
	(23, 'followers_of_lord_nyax'),
	(24, 'fs_villager'),
	(25, 'garyn'),
	(26, 'gondula_tribe'),
	(27, 'gorax'),
	(28, 'gungan'),
	(29, 'hidden_daggers'),
	(30, 'hsskor'),
	(31, 'hutt'),
	(32, 'jabba'),
	(33, 'janta_tribe'),
	(34, 'jawa'),
	(35, 'kobola'),
	(36, 'korga_tribe'),
	(37, 'kunga_tribe'),
	(38, 'liberation_party'),
	(39, 'lok_mercenaries'),
	(40, 'lost_aqualish'),
	(41, 'marauder'),
	(42, 'meatlump'),
	(43, 'mokk_tribe'),
	(44, 'monumenter'),
	(45, 'mtn_clan'),
	(46, 'naboo'),
	(47, 'naboo_pirate'),
	(48, 'naboo_security_force'),
	(49, 'narmle'),
	(50, 'nightsister'),
	(51, 'nym'),
	(52, 'olag_greck'),
	(53, 'panshee_tribe'),
	(54, 'pirate'),
	(55, 'plasma_thief'),
	(56, 'pubam'),
	(57, 'restuss'),
	(58, 'rogue_corsec'),
	(59, 'rorgungan'),
	(60, 'sif'),
	(61, 'sith_shadow'),
	(62, 'sith_shadow_nonaggro'),
	(63, 'smashball'),
	(64, 'spice_collective'),
	(65, 'spider_nightsister'),
	(66, 'swamp_rat'),
	(67, 'swoop'),
	(68, 'thug'),
	(69, 'townsperson'),
	(70, 'trade_federation'),
	(71, 'tusken_raider'),
	(72, 'valarian'),
	(73, 'warren_cyborg'),
	(74, 'warren_huurton'),
	(75, 'warren_imperial'),
	(76, 'warren_imperial_worker'),
	(77, 'warren_scientist'),
	(78, 'warren_teraud');
/*!40000 ALTER TABLE `faction` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
