-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:01
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.tangibles_defenders
DROP TABLE IF EXISTS `tangibles_defenders`;
CREATE TABLE IF NOT EXISTS `tangibles_defenders` (
  `tangible_id` bigint(20) NOT NULL,
  PRIMARY KEY (`tangible_id`),
  KEY `IDX_C6524BE2C7FE2A7F` (`tangible_id`),
  CONSTRAINT `FK_C6524BE2C7FE2A7F` FOREIGN KEY (`tangible_id`) REFERENCES `tangible` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.tangibles_defenders: ~0 rows (approximately)
/*!40000 ALTER TABLE `tangibles_defenders` DISABLE KEYS */;
/*!40000 ALTER TABLE `tangibles_defenders` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
