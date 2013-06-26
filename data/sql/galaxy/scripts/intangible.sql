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

-- Dumping structure for table galaxy.intangible
DROP TABLE IF EXISTS `intangible`;
CREATE TABLE IF NOT EXISTS `intangible` (
  `id` bigint(20) NOT NULL,
  `stf_detail_file` varchar(255) NOT NULL,
  `stf_detail_string` varchar(255) NOT NULL,
  `generic_int` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_BF03160DBF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
