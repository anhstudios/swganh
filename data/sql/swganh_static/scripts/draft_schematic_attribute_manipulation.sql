-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:16
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.draft_schematic_attribute_manipulation
DROP TABLE IF EXISTS `draft_schematic_attribute_manipulation`;
CREATE TABLE IF NOT EXISTS `draft_schematic_attribute_manipulation` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `Draft_Schematic` int(10) unsigned NOT NULL,
  `Attribute` int(10) unsigned NOT NULL,
  `AffectedAttribute` varchar(45) NOT NULL,
  `Manipulation` int(10) unsigned NOT NULL,
  `list_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.draft_schematic_attribute_manipulation: ~24 rows (approximately)
/*!40000 ALTER TABLE `draft_schematic_attribute_manipulation` DISABLE KEYS */;
INSERT INTO `draft_schematic_attribute_manipulation` (`ID`, `Draft_Schematic`, `Attribute`, `AffectedAttribute`, `Manipulation`, `list_id`) VALUES
	(8, 140, 613, '303', 1, 113),
	(9, 140, 185, '185', 1, 114),
	(10, 304, 613, '366', 1, 2137),
	(11, 304, 613, '367', 1, 2137),
	(12, 304, 185, '204', 1, 2138),
	(13, 152, 613, '9999', 0, 134),
	(14, 1073, 15, '303', 1, 63),
	(15, 890, 94, '460', 1, 1),
	(17, 269, 613, '372', 1, 129),
	(18, 269, 185, '204', 1, 130),
	(19, 279, 613, '375', 1, 131),
	(20, 279, 185, '204', 1, 132),
	(21, 300, 613, '367', 1, 76),
	(22, 300, 613, '366', 1, 76),
	(23, 300, 185, '204', 1, 78),
	(25, 294, 185, '23', 0, 148),
	(26, 270, 613, '372', 1, 137),
	(27, 270, 185, '204', 1, 138),
	(28, 274, 613, '373', 1, 139),
	(29, 274, 185, '23', 1, 140),
	(30, 280, 613, '375', 1, 141),
	(31, 280, 185, '23', 1, 142),
	(32, 284, 613, '377', 1, 143),
	(33, 284, 185, '23', 1, 144);
/*!40000 ALTER TABLE `draft_schematic_attribute_manipulation` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
