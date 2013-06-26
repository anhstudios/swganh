-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.27-MariaDB-log - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-22 09:15:11
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.object_attributes
DROP TABLE IF EXISTS `object_attributes`;
CREATE TABLE IF NOT EXISTS `object_attributes` (
  `object_id` bigint(20) DEFAULT NULL,
  `attribute_id` int(11) unsigned DEFAULT NULL,
  `attribute_value` varchar(255) DEFAULT '',
  PRIMARY KEY (`object_id`, `attribute_id`),
  KEY `FK_object_attributes_swganh_static.attributes` (`attribute_id`),
  CONSTRAINT `FK_object_attributes_swganh_static.attributes` FOREIGN KEY (`attribute_id`) REFERENCES `swganh_static`.`attributes` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='stores attributes for objects';

DELETE FROM `object_attributes`;

/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
