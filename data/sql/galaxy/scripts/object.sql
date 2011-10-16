# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 12:33:05
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.object
DROP TABLE IF EXISTS `object`;
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
  `discr` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_A8ADABEC166053B4` (`scene_id`),
  KEY `IDX_A8ADABEC727ACA70` (`parent_id`),
  CONSTRAINT `FK_A8ADABEC166053B4` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`),
  CONSTRAINT `FK_A8ADABEC727ACA70` FOREIGN KEY (`parent_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.object: ~19 rows (approximately)
/*!40000 ALTER TABLE `object` DISABLE KEYS */;
INSERT INTO `object` (`id`, `scene_id`, `parent_id`, `iff_template_id`, `x_position`, `y_position`, `z_position`, `x_orientation`, `y_orientation`, `z_orientation`, `w_orientation`, `complexity`, `stf_name_file`, `stf_name_string`, `custom_name`, `volume`, `created_at`, `updated_at`, `deleted_at`, `discr`) VALUES
	(0, 1, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'universe', 'universe', 'universe', 0, '2011-10-15 12:46:23', '2011-10-15 12:46:24', '2011-10-15 12:46:24', 'universe'),
	(8589934593, 1, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', NULL, ''),
	(8589934598, 1, 0, 1221, -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/human_male.iff', 'object/creature/player/human_male.iff', '0', 0, '2011-10-15 12:59:08', '2011-10-15 12:59:08', NULL, 'player character'),
	(8589934603, 1, 0, 1207, -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/human_male.iff', 'object/creature/player/human_male.iff', 'Wottik Oboo', 0, '2011-10-15 13:04:31', '2011-10-15 13:04:31', NULL, 'player character');
	(8589934613, 1, 0, 'object/creature/player/zabrak_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/zabrak_male.iff', 'object/creature/player/zabrak_male.iff', 'Kronos Ownz', 0, '2011-10-15 20:09:37', '2011-10-15 20:09:37', NULL, '0'),
	(8589934623, 1, 0, 'object/creature/player/zabrak_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/zabrak_male.iff', 'object/creature/player/zabrak_male.iff', 'Kronos Ownz', 0, '2011-10-15 20:11:05', '2011-10-15 20:11:05', NULL, '0'),
	(8589934633, 1, 0, 'object/creature/player/zabrak_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/zabrak_male.iff', 'object/creature/player/zabrak_male.iff', 'Kronos Ownz', 0, '2011-10-15 20:11:18', '2011-10-15 20:11:18', NULL, '0'),
	(8589934643, 1, 0, 'object/creature/player/zabrak_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/zabrak_male.iff', 'object/creature/player/zabrak_male.iff', 'Kronos Ownz', 0, '2011-10-15 20:12:10', '2011-10-15 20:12:10', NULL, '0'),
	(8589934644, 1, 0, 'object/creature/player/zabrak_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/zabrak_male.iff', 'object/creature/player/zabrak_male.iff', 'Kronos Ownz', 0, '2011-10-15 20:12:10', '2011-10-15 20:12:10', NULL, 'player character'),
	(8589934645, 1, 0, 'object/creature/player/zabrak_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/zabrak_male.iff', 'object/creature/player/zabrak_male.iff', 'Kronos Ownz', 0, '2011-10-15 20:12:10', '2011-10-15 20:12:10', NULL, '0'),
	(8589934649, 1, 0, 'object/tangible/hair/zabrak/hair_zabrak_male_s03.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/tangible/hair/zabrak/hair_zabrak_male_s03.iff', 'object/tangible/hair/zabrak/hair_zabrak_male_s03.iff', 'hair', 0, '2011-10-15 20:12:10', '2011-10-15 20:12:10', NULL, '0'),
	(8589934659, 1, 0, 'object/creature/player/human_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/human_male.iff', 'object/creature/player/human_male.iff', 'Ohyfo Ryso', 0, '2011-10-15 20:15:10', '2011-10-15 20:15:10', NULL, '0'),
	(8589934660, 1, 0, 'object/creature/player/human_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/human_male.iff', 'object/creature/player/human_male.iff', 'Ohyfo Ryso', 0, '2011-10-15 20:15:10', '2011-10-15 20:15:10', NULL, 'player character'),
	(8589934661, 1, 0, 'object/creature/player/human_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/human_male.iff', 'object/creature/player/human_male.iff', 'Ohyfo Ryso', 0, '2011-10-15 20:15:10', '2011-10-15 20:15:10', NULL, '0'),
	(8589934665, 1, 0, 'object/tangible/hair/human/hair_human_male_s40.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/tangible/hair/human/hair_human_male_s40.iff', 'object/tangible/hair/human/hair_human_male_s40.iff', 'hair', 0, '2011-10-15 20:15:10', '2011-10-15 20:15:10', NULL, '0'),
	(8589934675, 1, 0, 'object/creature/player/moncal_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/moncal_male.iff', 'object/creature/player/moncal_male.iff', 'Isshym', 0, '2011-10-15 22:18:17', '2011-10-15 22:18:17', NULL, '0'),
	(8589934676, 1, 0, 'object/creature/player/moncal_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/moncal_male.iff', 'object/creature/player/moncal_male.iff', 'Isshym', 0, '2011-10-15 22:18:17', '2011-10-15 22:18:17', NULL, 'player character'),
	(8589934677, 1, 0, 'object/creature/player/moncal_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/moncal_male.iff', 'object/creature/player/moncal_male.iff', 'Isshym', 0, '2011-10-15 22:18:17', '2011-10-15 22:18:17', NULL, '0'),
	(8589934681, 1, 0, '', -137, 0, -4723, 0, 1, 0, 0, 0, '', '', 'hair', 0, '2011-10-15 22:18:17', '2011-10-15 22:18:17', NULL, '0'),
	(8589934691, 1, 0, 'object/creature/player/trandoshan_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/trandoshan_male.iff', 'object/creature/player/trandoshan_male.iff', 'Ocketo Alollig', 0, '2011-10-15 22:43:33', '2011-10-15 22:43:33', NULL, '0'),
	(8589934692, 1, 8589934691, 'object/creature/player/trandoshan_male.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/creature/player/trandoshan_male.iff', 'object/creature/player/trandoshan_male.iff', 'Ocketo Alollig', 0, '2011-10-15 22:43:33', '2011-10-15 22:43:33', NULL, 'player character'),
	(8589934697, 1, 0, 'object/tangible/hair/trandoshan/hair_trandoshan_male_s03.iff', -137, 0, -4723, 0, 1, 0, 0, 0, 'object/tangible/hair/trandoshan/hair_trandoshan_male_s03.iff', 'object/tangible/hair/trandoshan/hair_trandoshan_male_s03.iff', 'hair', 0, '2011-10-15 22:43:33', '2011-10-15 22:43:33', NULL, '0');
/*!40000 ALTER TABLE `object` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
