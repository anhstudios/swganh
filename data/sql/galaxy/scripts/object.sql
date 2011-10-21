# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-21 14:46:05
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.object
CREATE TABLE IF NOT EXISTS `object` (
  `id` bigint(20) NOT NULL DEFAULT '8589934593',
  `scene_id` int(11) DEFAULT NULL,
  `parent_id` bigint(20) DEFAULT NULL,
  `iff_template_id` int(10) unsigned NOT NULL,
  `x_position` double NOT NULL,
  `y_position` double NOT NULL,
  `z_position` double NOT NULL,
  `x_orientation` double NOT NULL,
  `y_orientation` double NOT NULL,
  `z_orientation` double NOT NULL,
  `w_orientation` double NOT NULL,
  `complexity` double NOT NULL,
  `stf_name_file` varchar(255) NOT NULL,
  `stf_name_string` varchar(255) NOT NULL,
  `custom_name` varchar(255) NOT NULL,
  `volume` int(11) NOT NULL,
  `created_at` datetime NOT NULL,
  `updated_at` datetime NOT NULL,
  `deleted_at` datetime DEFAULT NULL,
  `type_id` int(10) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `IDX_A8ADABEC166053B4` (`scene_id`),
  KEY `IDX_A8ADABEC727ACA70` (`parent_id`),
  KEY `IDX_A8ADABEC1220ACE89` (`type_id`),
  CONSTRAINT `FK_A8ADABEC166053B4` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`),
  CONSTRAINT `FK_A8ADABEC727ACA70` FOREIGN KEY (`parent_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.object: ~17 rows (approximately)
DELETE FROM `object`;
/*!40000 ALTER TABLE `object` DISABLE KEYS */;
INSERT INTO `object` (`id`, `scene_id`, `parent_id`, `iff_template_id`, `x_position`, `y_position`, `z_position`, `x_orientation`, `y_orientation`, `z_orientation`, `w_orientation`, `complexity`, `stf_name_file`, `stf_name_string`, `custom_name`, `volume`, `created_at`, `updated_at`, `deleted_at`, `type_id`) VALUES
	(0, 1, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'universe', 'universe', 'universe', 0, '2011-10-15 12:46:23', '2011-10-15 12:46:24', '2011-10-15 12:46:24', 1129465167),
	(8589934593, 1, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', NULL, 1129465167),
	(8589934603, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Keerippei Eelkoed', 0, '2011-10-21 13:11:58', '2011-10-21 13:11:58', NULL, 1129465167),
	(8589934604, 1, 8589934603, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Keerippei Eelkoed', 0, '2011-10-21 13:11:58', '2011-10-21 13:11:58', NULL, 1347174745),
	(8589934609, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 13:11:58', '2011-10-21 13:11:58', NULL, 1413566031),
	(8589934619, 1, 0, 1213, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_rodian', 'Rasliss Osskrimu', 0, '2011-10-21 13:12:28', '2011-10-21 13:12:28', NULL, 1129465167),
	(8589934620, 1, 8589934619, 1213, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_rodian', 'Rasliss Osskrimu', 0, '2011-10-21 13:12:28', '2011-10-21 13:12:28', NULL, 1347174745),
	(8589934625, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 13:12:28', '2011-10-21 13:12:28', NULL, 1413566031),
	(8589934635, 1, 0, 1213, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_rodian', 'Otromo Ussotro', 0, '2011-10-21 13:12:44', '2011-10-21 13:12:44', NULL, 1129465167),
	(8589934636, 1, 8589934635, 1213, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_rodian', 'Otromo Ussotro', 0, '2011-10-21 13:12:44', '2011-10-21 13:12:44', NULL, 1347174745),
	(8589934641, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 13:12:44', '2011-10-21 13:12:44', NULL, 1413566031),
	(8589934651, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Oamik Oanakag', 0, '2011-10-21 13:49:47', '2011-10-21 13:49:47', NULL, 1129465167),
	(8589934652, 1, 8589934651, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Oamik Oanakag', 0, '2011-10-21 13:49:47', '2011-10-21 13:49:47', NULL, 1347174745),
	(8589934657, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 13:49:47', '2011-10-21 13:49:47', NULL, 1413566031),
	(8589934667, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Audiglig Droulilke', 0, '2011-10-21 13:57:37', '2011-10-21 13:57:37', NULL, 1129465167),
	(8589934668, 1, 8589934667, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Audiglig Droulilke', 0, '2011-10-21 13:57:37', '2011-10-21 13:57:37', NULL, 1347174745),
	(8589934673, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 13:57:37', '2011-10-21 13:57:37', NULL, 1413566031),
	(8589934683, 1, 0, 1223, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_zabrak', 'Zobarky zarag', 0, '2011-10-21 14:38:38', '2011-10-21 14:38:38', NULL, 1129465167),
	(8589934684, 1, 8589934683, 1223, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_zabrak', 'Zobarky zarag', 0, '2011-10-21 14:38:38', '2011-10-21 14:38:38', NULL, 1347174745),
	(8589934689, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 14:38:38', '2011-10-21 14:38:38', NULL, 1413566031);
/*!40000 ALTER TABLE `object` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
