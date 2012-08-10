
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

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
  `type_id` int(10) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_A8ADABEC166053B4` (`scene_id`),
  KEY `IDX_A8ADABEC727ACA70` (`parent_id`),
  KEY `IDX_A8ADABEC1220ACE89` (`type_id`),
  CONSTRAINT `FK_A8ADABEC166053B4` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`),
  CONSTRAINT `FK_A8ADABEC727ACA70` FOREIGN KEY (`parent_id`) REFERENCES `object` (`id`),
  CONSTRAINT `FK_A8ADABEC727AB45A` FOREIGN KEY (`iff_template_id`) REFERENCES `iff_templates` (`id`),
  CONSTRAINT `FK_A8ADABEC1220ACE89` FOREIGN KEY (`type_id`) REFERENCES `types` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `object`;
    
/*!40000 ALTER TABLE `object` DISABLE KEYS */;
INSERT INTO `object` (`id`, `scene_id`, `parent_id`, `iff_template_id`, `x_position`, `y_position`, `z_position`, `x_orientation`, `y_orientation`, `z_orientation`, `w_orientation`, `complexity`, `stf_name_file`, `stf_name_string`, `custom_name`, `volume`, `created_at`, `updated_at`, `deleted_at`, `type_id`) VALUES
	(0, 1, NULL, 15495, 0, 0, 0, 0, 0, 0, 0, 0, 'universe', 'universe', 'universe', 0, '2011-10-21 23:00:59', '2011-10-21 23:01:00', '0000-00-00 00:00:00', NULL),
	(8589934593, 1, 0, 1203, 0, 0, 0, 0, 0, 0, 0, 0, 'player', 'example', 'player', 0, '2011-10-21 23:02:22', '2011-10-21 23:02:23', '0000-00-00 00:00:00', 1347174745);
/*!40000 ALTER TABLE `object` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
