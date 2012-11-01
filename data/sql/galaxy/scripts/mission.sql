-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:58
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.mission
DROP TABLE IF EXISTS `mission`;
CREATE TABLE IF NOT EXISTS `mission` (
  `id` bigint(20) NOT NULL,
  `waypoint_id` bigint(20) DEFAULT NULL,
  `difficulty_level` int(11) NOT NULL,
  `start_position_x` double NOT NULL,
  `start_position_y` double NOT NULL,
  `start_position_z` double NOT NULL,
  `start_scene` int(11) NOT NULL,
  `creator` varchar(255) NOT NULL,
  `reward` int(11) NOT NULL,
  `destination_position_x` double NOT NULL,
  `destination_position_y` double NOT NULL,
  `destination_position_z` double NOT NULL,
  `destination_scene` int(11) NOT NULL,
  `description_stf_file` varchar(255) NOT NULL,
  `description_stf_name` varchar(255) NOT NULL,
  `title_stf_file` varchar(255) NOT NULL,
  `title_stf_name` varchar(255) NOT NULL,
  `repeat_counter` int(11) NOT NULL,
  `mission_type` varchar(255) NOT NULL,
  `target_name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQ_9067F23C7BB1FD97` (`waypoint_id`),
  CONSTRAINT `FK_9067F23C7BB1FD97` FOREIGN KEY (`waypoint_id`) REFERENCES `waypoint` (`id`),
  CONSTRAINT `FK_9067F23CBF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.mission: ~0 rows (approximately)
/*!40000 ALTER TABLE `mission` DISABLE KEYS */;
/*!40000 ALTER TABLE `mission` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
