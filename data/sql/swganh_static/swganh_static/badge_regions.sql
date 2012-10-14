-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:15
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.badge_regions
DROP TABLE IF EXISTS `badge_regions`;
CREATE TABLE IF NOT EXISTS `badge_regions` (
  `id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Badge Region ID',
  `region_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Planet Region ID',
  `badge_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Badge ID',
  `planet_id` int(11) NOT NULL DEFAULT '0' COMMENT 'Planet ID',
  `parent_id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'Parent ID',
  PRIMARY KEY (`id`),
  KEY `fk_badge_regions_region_region` (`region_id`),
  KEY `fk_badge_regions_badge_badge` (`badge_id`),
  KEY `fk_badgeregion_planet_planet` (`planet_id`),
  CONSTRAINT `badge_id` FOREIGN KEY (`badge_id`) REFERENCES `badges` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `badge_planet_id` FOREIGN KEY (`planet_id`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=REDUNDANT;

-- Dumping data for table swganh_static.badge_regions: ~46 rows (approximately)
/*!40000 ALTER TABLE `badge_regions` DISABLE KEYS */;
INSERT INTO `badge_regions` (`id`, `region_id`, `badge_id`, `planet_id`, `parent_id`) VALUES
	(64692944896, 38, 79, 0, 0),
	(64692944897, 39, 80, 0, 0),
	(64692944898, 40, 81, 0, 0),
	(64692944899, 41, 20, 0, 0),
	(64692944900, 42, 78, 0, 0),
	(64692944901, 43, 84, 1, 0),
	(64692944902, 44, 85, 1, 0),
	(64692944903, 45, 31, 1, 0),
	(64692944904, 46, 30, 1, 0),
	(64692944905, 47, 83, 2, 0),
	(64692944906, 48, 25, 2, 0),
	(64692944907, 49, 26, 2, 0),
	(64692944908, 50, 27, 2, 0),
	(64692944909, 51, 28, 2, 0),
	(64692944910, 52, 82, 2, 0),
	(64692944911, 53, 29, 2, 0),
	(64692944912, 54, 86, 3, 0),
	(64692944913, 55, 88, 3, 0),
	(64692944914, 56, 87, 3, 0),
	(64692944915, 57, 89, 3, 0),
	(64692944916, 58, 98, 4, 0),
	(64692944917, 59, 24, 4, 0),
	(64692944918, 60, 99, 4, 0),
	(64692944919, 61, 75, 5, 0),
	(64692944920, 62, 76, 5, 0),
	(64692944921, 63, 77, 5, 0),
	(64692944922, 64, 19, 5, 0),
	(64692944923, 65, 97, 6, 0),
	(64692944924, 66, 96, 6, 0),
	(64692944925, 67, 95, 6, 0),
	(64692944926, 68, 94, 6, 0),
	(64692944927, 69, 90, 7, 0),
	(64692944928, 70, 91, 7, 0),
	(64692944929, 71, 92, 7, 0),
	(64692944930, 72, 93, 7, 0),
	(64692944931, 73, 17, 8, 0),
	(64692944932, 74, 12, 8, 0),
	(64692944933, 75, 16, 8, 0),
	(64692944934, 76, 15, 8, 0),
	(64692944935, 77, 14, 8, 0),
	(64692944936, 78, 18, 8, 0),
	(64692944937, 79, 13, 8, 1189178),
	(64692944938, 80, 23, 9, 0),
	(64692944939, 81, 22, 9, 0),
	(64692944940, 82, 21, 9, 0),
	(64692944941, 85, 140, 42, 0);
/*!40000 ALTER TABLE `badge_regions` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
