-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:18
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.parking_garage
DROP TABLE IF EXISTS `parking_garage`;
CREATE TABLE IF NOT EXISTS `parking_garage` (
  `id` bigint(12) NOT NULL,
  `parent` bigint(20) NOT NULL,
  `oX` float NOT NULL,
  `oY` float NOT NULL,
  `oZ` float NOT NULL,
  `oW` float NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `planet` int(2) NOT NULL,
  `radius` bigint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_planet` (`planet`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.parking_garage: ~22 rows (approximately)
/*!40000 ALTER TABLE `parking_garage` DISABLE KEYS */;
INSERT INTO `parking_garage` (`id`, `parent`, `oX`, `oY`, `oZ`, `oW`, `x`, `y`, `z`, `planet`, `radius`) VALUES
	(800010, 0, 0, 1, 0, 0, 409, 45, 3712, 42, 0),
	(1053331, 0, 0, 0, 0, 1, 3773.82, 8.39, 2326.82, 8, 0),
	(1153528, 0, 0, 0, 0, 1, 1776.85, 7, 2085.44, 8, 0),
	(1198831, 0, 0, 0.43, 0, 0.9, 3371.11, 5, -4964.09, 8, 0),
	(1232911, 0, 0, 0, 0, 1, 1456.73, 7, 3471.41, 8, 0),
	(1254820, 0, 0, 0.15, 0, 0.99, -2735.38, 5, 2283.36, 8, 0),
	(1256245, 0, 0, 0.99, 0, -0.12, -2856.46, 5, 1923.5, 8, 0),
	(1259838, 0, 0, 0.92, 0, 0.4, -2970.24, 5, 2514.27, 8, 0),
	(1261558, 0, 0, 0.61, 0, 0.8, -3023.59, 5, 1912.97, 8, 0),
	(1281822, 0, 0, 0.96, 0, 0.29, 3526.88, 5, -4640.47, 8, 0),
	(1677428, 0, 0, 0.71, 0, -0.71, -5656.09, 6, 4111.74, 5, 0),
	(1692371, 0, 0, 1, 0, 0, 1647.42, 12, 2426.15, 5, 0),
	(1717499, 0, 0, 0.8, 0, 0.61, 4646.82, 3.75, -4771.36, 5, 0),
	(1741554, 0, 0, 0.37, 0, 0.93, 5109.11, -192, 6573.89, 5, 0),
	(1855498, 0, 0, 0, 0, 1, -263.2, 28, -4790.78, 0, 0),
	(2115635, 0, 0, 1, 0, 0, 43.62, 28, -4256.46, 0, 0),
	(2116099, 0, 0, 0, 0, 1, -5567.06, 21, -2562.82, 0, 0),
	(3005393, 0, 0, -0.42, 0, 0.91, -3184.25, 31, 2818.83, 0, 0),
	(3175575, 0, 0, 0.7, 0, 0.71, 167.16, 6, -3085.3, 6, 0),
	(3175576, 0, 0, 0.71, 0, 0.71, 660, 6, -2870, 6, 0),
	(3555528, 0, 0, 1, 0, 0, 2527.3, 13.61, 1696.3, 5, 0),
	(4635587, 0, 0, 0, 0, 1, -5233.81, 79.8, -2479.73, 6, 0);
/*!40000 ALTER TABLE `parking_garage` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
