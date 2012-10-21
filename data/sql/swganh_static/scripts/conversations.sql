-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:16
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.conversations
DROP TABLE IF EXISTS `conversations`;
CREATE TABLE IF NOT EXISTS `conversations` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `description` varchar(1024) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=utf8 COMMENT='Conversations (NPC)';

-- Dumping data for table swganh_static.conversations: ~40 rows (approximately)
/*!40000 ALTER TABLE `conversations` DISABLE KEYS */;
INSERT INTO `conversations` (`id`, `description`) VALUES
	(1, 'brawler trainer dialog'),
	(2, 'entertainer trainer dialog'),
	(3, 'scout trainer dialog'),
	(4, 'medic trainer dialog'),
	(5, 'artisan trainer dialog'),
	(6, 'marksman trainer dialog'),
	(7, 'rifleman trainer dialog'),
	(8, 'pistol trainer dialog'),
	(9, 'carbine trainer dialog'),
	(10, 'unarmed trainer dialog'),
	(11, 'fencer trainer dialog'),
	(12, 'swordman trainer dialog'),
	(13, 'polearm trainer dialog'),
	(14, 'dancer trainer dialog'),
	(15, 'musician trainer dialog'),
	(16, 'doctor trainer dialog'),
	(17, 'ranger trainer dialog'),
	(18, 'creature handler dialog'),
	(19, 'bioengineer trainer dialog'),
	(20, 'armorsmith trainer dialog'),
	(21, 'weaponsmith trainer dialog'),
	(22, 'chef trainer dialog'),
	(23, 'tailor trainer dialog'),
	(24, 'architect trainer dialog'),
	(25, 'droidengineer trainer dialog'),
	(26, 'merchant trainer dialog'),
	(27, 'smuggler trainer dialog'),
	(28, 'bountyhunter trainer dialog'),
	(29, 'commando trainer dialog'),
	(30, 'combat medic trainer dialog'),
	(31, 'image designer trainer dialog'),
	(32, 'squad leader trainer dialog'),
	(33, 'politician trainer dialog'),
	(34, 'tutorial item room imperial officer dialog'),
	(35, 'tutorial bank and bazzar room imperial officer dialog'),
	(36, 'tutorial cloning and insurance room droid dialog'),
	(37, 'tutorial covard imperial officer room dialog'),
	(38, 'tutorial imperial officer with trainer room dialog'),
	(39, 'tutorial imperial officer in mission terminal room dialog'),
	(40, 'tutorial quartermaster room dialog');
/*!40000 ALTER TABLE `conversations` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
