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

-- Dumping structure for table galaxy.manufacture_schematic
DROP TABLE IF EXISTS `manufacture_schematic`;
CREATE TABLE IF NOT EXISTS `manufacture_schematic` (
  `id` bigint(20) NOT NULL,
  `quantity` int(11) NOT NULL,
  `creator` varchar(255) NOT NULL,
  `data_size` double NOT NULL,
  `customization` varchar(255) NOT NULL,
  `customization_model` varchar(255) NOT NULL,
  `prototype_model` varchar(255) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `risk_factor` double NOT NULL,
  `is_ready` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_51DC8E01BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.manufacture_schematic: ~0 rows (approximately)
/*!40000 ALTER TABLE `manufacture_schematic` DISABLE KEYS */;
/*!40000 ALTER TABLE `manufacture_schematic` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
