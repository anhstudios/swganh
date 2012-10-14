-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:20
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.weapon_groups
DROP TABLE IF EXISTS `weapon_groups`;
CREATE TABLE IF NOT EXISTS `weapon_groups` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `defaultAttackAnimationCrc` int(11) unsigned NOT NULL DEFAULT '0',
  `defaultCombatSpam` varchar(255) NOT NULL DEFAULT 'melee',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=513 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.weapon_groups: ~10 rows (approximately)
/*!40000 ALTER TABLE `weapon_groups` DISABLE KEYS */;
INSERT INTO `weapon_groups` (`id`, `name`, `defaultAttackAnimationCrc`, `defaultCombatSpam`) VALUES
	(1, 'unarmed', 1136984016, 'melee'),
	(2, '1h', 1136984016, 'melee'),
	(4, '2h', 1136984016, 'melee'),
	(8, 'polearm', 1136984016, 'melee'),
	(16, 'rifle', 1349426508, 'shoot'),
	(32, 'pistol', 1349426508, 'shoot'),
	(64, 'carbine', 1349426508, 'shoot'),
	(128, 'flamethrower', 1349426508, 'shoot'),
	(256, 'saber', 1136984016, 'melee'),
	(512, 'launcher', 1349426508, 'shoot');
/*!40000 ALTER TABLE `weapon_groups` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
