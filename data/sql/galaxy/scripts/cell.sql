-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:57
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.cell
DROP TABLE IF EXISTS `cell`;
CREATE TABLE IF NOT EXISTS `cell` (
  `id` bigint(20) NOT NULL,
  `cell_number` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_CB8787E2BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table galaxy.cell: ~3 rows (approximately)
/*!40000 ALTER TABLE `cell` DISABLE KEYS */;
INSERT INTO `cell` (`id`, `cell_number`) VALUES
	(1038, 0),
	(1039, 0),
	(1040, 0);
/*!40000 ALTER TABLE `cell` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
