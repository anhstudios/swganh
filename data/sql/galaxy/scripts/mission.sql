
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

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
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `mission`;
    
/*!40000 ALTER TABLE `mission` DISABLE KEYS */;
/*!40000 ALTER TABLE `mission` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
