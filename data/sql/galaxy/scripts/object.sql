-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.24-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win32
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-08-12 16:17:22
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.object
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
  `type_id` int(10) DEFAULT NULL,
  `arrangement_id` int(10) DEFAULT '-2',
  `permission_type` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `IDX_A8ADABEC166053B4` (`scene_id`),
  KEY `IDX_A8ADABEC727ACA70` (`parent_id`),
  KEY `IDX_A8ADABEC1220ACE89` (`type_id`),
  KEY `FK_A8ADABEC727AB45A` (`iff_template_id`),
  KEY `permission_type` (`permission_type`),
  CONSTRAINT `FK_A8ADABEC1220ACE89` FOREIGN KEY (`type_id`) REFERENCES `types` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_A8ADABEC166053B4` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`),
  CONSTRAINT `FK_A8ADABEC727AB45A` FOREIGN KEY (`iff_template_id`) REFERENCES `iff_templates` (`id`),
  CONSTRAINT `FK_A8ADABEC727ACA70` FOREIGN KEY (`parent_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
