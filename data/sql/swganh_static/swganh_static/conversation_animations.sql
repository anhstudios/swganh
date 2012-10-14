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

-- Dumping structure for table swganh_static.conversation_animations
DROP TABLE IF EXISTS `conversation_animations`;
CREATE TABLE IF NOT EXISTS `conversation_animations` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Conversation Animation ID',
  `name` varchar(255) NOT NULL DEFAULT 'none' COMMENT 'Conversation Animation Name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=utf8 COMMENT='Conversation Animations (NPC)';

-- Dumping data for table swganh_static.conversation_animations: ~46 rows (approximately)
/*!40000 ALTER TABLE `conversation_animations` DISABLE KEYS */;
INSERT INTO `conversation_animations` (`id`, `name`) VALUES
	(1, 'bow'),
	(2, 'beckon'),
	(3, 'adn'),
	(4, 'apologize'),
	(5, 'ayt'),
	(6, 'backhand'),
	(7, 'blame'),
	(8, 'bow2'),
	(9, 'bow3'),
	(10, 'bow4'),
	(11, 'claw'),
	(12, 'cuckoo'),
	(13, 'curtsey'),
	(14, 'dream'),
	(15, 'rose'),
	(16, 'giveup'),
	(17, 'helpme'),
	(18, 'huge'),
	(19, 'medium'),
	(20, 'small'),
	(21, 'tiny'),
	(22, 'jam'),
	(23, 'loser'),
	(24, 'mock'),
	(25, 'model'),
	(26, 'nod'),
	(27, 'poke'),
	(28, 'rude'),
	(29, 'scare'),
	(30, 'scared'),
	(31, 'scream'),
	(32, 'shiver'),
	(33, 'shoo'),
	(34, 'snap'),
	(35, 'spin'),
	(36, 'squirm'),
	(37, 'strut'),
	(38, 'sweat'),
	(39, 'thank'),
	(40, 'throwdown'),
	(41, 'tiphat'),
	(42, 'twitch'),
	(43, 'worship'),
	(44, 'yawn'),
	(45, 'yes'),
	(46, 'none');
/*!40000 ALTER TABLE `conversation_animations` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
