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

-- Dumping structure for table swganh_static.npc_mood_animations
DROP TABLE IF EXISTS `npc_mood_animations`;
CREATE TABLE IF NOT EXISTS `npc_mood_animations` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `animation_type` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=utf8 COMMENT='animations for the npcs';

-- Dumping data for table swganh_static.npc_mood_animations: ~23 rows (approximately)
/*!40000 ALTER TABLE `npc_mood_animations` DISABLE KEYS */;
INSERT INTO `npc_mood_animations` (`id`, `animation_type`) VALUES
	(1, 'bored'),
	(2, 'calm'),
	(3, 'conversation'),
	(4, 'default'),
	(5, 'explain'),
	(6, 'none'),
	(7, 'neutral'),
	(8, 'npc_accusing'),
	(9, 'npc_angry'),
	(10, 'npc_consoling'),
	(11, 'npc_dead_01'),
	(12, 'npc_dead_02'),
	(13, 'npc_dead_03'),
	(14, 'npc_imperial'),
	(15, 'npc_sad'),
	(16, 'npc_sitting_chair'),
	(17, 'npc_sitting_ground'),
	(18, 'npc_sitting_table'),
	(19, 'npc_sitting_table_eating'),
	(20, 'npc_standing_drinking'),
	(21, 'npc_use_console_ag'),
	(22, 'npc_use_terminal_high'),
	(23, 'npc_use_terminal_low');
/*!40000 ALTER TABLE `npc_mood_animations` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
