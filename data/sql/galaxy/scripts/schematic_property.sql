-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:00
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.schematic_property
DROP TABLE IF EXISTS `schematic_property`;
CREATE TABLE IF NOT EXISTS `schematic_property` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `manufacture_schematic_id` bigint(20) DEFAULT NULL,
  `stf_file` varchar(255) NOT NULL,
  `stf_name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_C21D7FB14DE6846C` (`manufacture_schematic_id`),
  CONSTRAINT `FK_C21D7FB14DE6846C` FOREIGN KEY (`manufacture_schematic_id`) REFERENCES `manufacture_schematic` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.schematic_property: ~0 rows (approximately)
/*!40000 ALTER TABLE `schematic_property` DISABLE KEYS */;
/*!40000 ALTER TABLE `schematic_property` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
