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

-- Dumping structure for table swganh_static.conversation_event_types
DROP TABLE IF EXISTS `conversation_event_types`;
CREATE TABLE IF NOT EXISTS `conversation_event_types` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `description` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.conversation_event_types: ~5 rows (approximately)
/*!40000 ALTER TABLE `conversation_event_types` DISABLE KEYS */;
INSERT INTO `conversation_event_types` (`id`, `description`) VALUES
	(0, 'none'),
	(1, 'learn skill'),
	(2, 'select_learn_skill'),
	(3, 'prepare_selection'),
	(4, 'tutorial_special_event');
/*!40000 ALTER TABLE `conversation_event_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
