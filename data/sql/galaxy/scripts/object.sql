# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-21 00:47:35
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
  CONSTRAINT `FK_A8ADABEC1220ACE89` FOREIGN KEY (`type_id`) REFERENCES `types` (`id`) ON DELETE NO ACTION,
  CONSTRAINT `FK_A8ADABEC166053B4` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`),
  CONSTRAINT `FK_A8ADABEC727ACA70` FOREIGN KEY (`parent_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.object: ~3 rows (approximately)
DELETE FROM `object`;
/*!40000 ALTER TABLE `object` DISABLE KEYS */;
INSERT INTO `object` (`id`, `scene_id`, `parent_id`, `iff_template_id`, `x_position`, `y_position`, `z_position`, `x_orientation`, `y_orientation`, `z_orientation`, `w_orientation`, `complexity`, `stf_name_file`, `stf_name_string`, `custom_name`, `volume`, `created_at`, `updated_at`, `deleted_at`, `type_id`) VALUES
	(0, 1, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'universe', 'universe', 'universe', 0, '2011-10-15 12:46:23', '2011-10-15 12:46:24', '2011-10-15 12:46:24', 1),
	(8589934593, 1, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', NULL, 1),
	(8589934603, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Hiofihoo Wacae', 0, '2011-10-21 00:45:18', '2011-10-21 00:45:18', NULL, 2),
	(8589934613, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Siwoaglik Adliathid', 0, '2011-10-21 00:46:08', '2011-10-21 00:46:08', NULL, 2),
	(8589934619, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 00:46:08', '2011-10-21 00:46:08', NULL, 3),
	(8589934629, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Siwoaglik Adliathid', 0, '2011-10-21 00:46:15', '2011-10-21 00:46:15', NULL, 2),
	(8589934635, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 00:46:15', '2011-10-21 00:46:15', NULL, 3),
	(8589934645, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Siwoaglik Adliathid', 0, '2011-10-21 00:46:30', '2011-10-21 00:46:30', NULL, 2),
	(8589934646, 1, 8589934645, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'player_species', 'name_human', 'Siwoaglik Adliathid', 0, '2011-10-21 00:46:30', '2011-10-21 00:46:30', NULL, 1),
	(8589934651, 1, 0, 0, -137, 0, -4723, 0, 1, 0, 0, 0, 'hair_detail', 'hair', 'hair', 0, '2011-10-21 00:46:30', '2011-10-21 00:46:30', NULL, 3);
/*!40000 ALTER TABLE `object` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
